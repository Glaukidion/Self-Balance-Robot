/* **********************************************************************/
/* File name:       StepMotorCrtl.cpp                                   */
/* File description: Step motor control implementation, using internal  */
/* timer to operate at controlled PWM frequency                         */
/* Author name:      Grupo 2                                            */
/* Creation date:    31ago2021                                          */
/* Revision date:    05dez2021                                          */
/* **********************************************************************/

#include "StepMotorCrtl.h"
#include <Arduino.h>

/* **********************************************************************/
/* Method name:        StepMotorCrtl                                  	*/
/* Method description: StepMotorCrtl class constructor						    	*/
/* Input params:       ucStepPin1 -> the step pin used by motor 1      	*/
/*										 ucStepPin2 -> the step pin used by motor 2				*/
/*										 ucDirPin1 -> the direction pin used by motor 1   */
/*										 ucDirPin2 -> the direction pin used by motor 2		*/
/*										 ucEnablePin1 -> the enables pin used by motor 1	*/
/*										 ucEnablePin2 -> the enables pin used by motor 2	*/
/* Output params:      n/a                                     					*/
/* **********************************************************************/
StepMotorCrtl::StepMotorCrtl(unsigned char ucStepPin1, unsigned char ucStepPin2, unsigned char ucDirPin1, unsigned char ucDirPin2,
  unsigned char ucEnablePin1, unsigned char ucEnablePin2){

    pinMode(ucStepPin1, OUTPUT);
    pinMode(ucStepPin2, OUTPUT);
    pinMode(ucDirPin1, OUTPUT);
    pinMode(ucDirPin2, OUTPUT);
    pinMode(ucEnablePin1, OUTPUT);
    pinMode(ucEnablePin2, OUTPUT);
    //stores the pins used by the motors
    ucMyStepPin1 = ucStepPin1;
    ucMyStepPin2 = ucStepPin2;
    ucMyDirPin1 = ucDirPin1;
    ucMyDirPin2 = ucDirPin2;
    ucMyEnablePin1 = ucEnablePin1;
    ucMyEnablePin2 = ucEnablePin2;
}
/* ***************************************************************/
/* Method name:        step_motors_init                          */
/* Method description: Initialize the counters used by the motors*/
/* Input params:       n/a                                       */
/* Output params:      n/a                                       */
/* ***************************************************************/
void StepMotorCrtl::step_motors_init(){
  /*8 bit Timer/Counter 0 initialization*/
  PRR &= ~(1 << PRTIM0_bit);            //Disable power reduction at Timer 0
  /*Set the timer 0 to operate at Fast - PWM mode - with TOP OCR0A register and toggle on compare*/
  TCCR0A |= 1 << TCCR0_2A_WGM00_bit;
  TCCR0A |= 1 << TCCR0_2A_WGM01_bit;
  TCCR0B |= 1 << TCCR0_2B_WGM02_bit;
  TCCR0A &= ~(1 << COM0_2A1_bit);
  TCCR0A |= 1 << COM0_2A0_bit;
  /*8 bit Timer/Counter 2 initialization*/
  PRR &= ~(1 << PRTIM2_bit);            //Disable power reduction at Timer 2
  /*Set the timer 2 to operate at Fast - PWM mode - with TOP OCR2A register and toggle on compare*/
  TCCR2A |= 1 << TCCR0_2A_WGM00_bit;
  TCCR2A |= 1 << TCCR0_2A_WGM01_bit;
  TCCR2B |= 1 << TCCR0_2B_WGM02_bit;
  TCCR2A &= ~(1 << COM0_2A1_bit);
  TCCR2A |= 1 << COM0_2A0_bit;
  /*Set the prescaler with maximum value*/
  set_prescaler(TMR0_1_1024_CLK);
  set_prescaler(TMR2_1_1024_CLK);
  /*Enables the motor 1*/
  digitalWrite(ucMyEnablePin1, LOW);
  /*Enables the motor 2*/
  digitalWrite(ucMyEnablePin2, LOW);
}
/* ***************************************************************/
/* Method name:        set_prescaler                             */
/* Method description: set the prescalers used by the timer 0    */
/* Input params:       uiTimer0_Prescaler -> the prescaler       */
/*                                                setting value  */
/* Output params:      n/a                                       */
/* ***************************************************************/
void StepMotorCrtl::set_prescaler(TIMER0_PRESCALER uiTimer0_Prescaler){
  /*Set the clock source prescaler*/
  TCCR0B &= 0xf8;                  //- first clears the three LSB bits*
  TCCR0B |= uiTimer0_Prescaler;    //mask the register bits
  /*Saves the prescaler values*/
  switch (uiTimer0_Prescaler) {
    case TMR0_1_8_CLK:
      uiStored_Timer0_prescaler = 8;
      break;
    case TMR0_1_64_CLK:
      uiStored_Timer0_prescaler = 64;
      break;
    case TMR0_1_256_CLK:
      uiStored_Timer0_prescaler = 256;
      break;
    case TMR0_1_1024_CLK:
      uiStored_Timer0_prescaler = 1024;
      break;
  }
}
/* ***************************************************************/
/* Method name:        set_prescaler                             */
/* Method description: set the prescalers used by the timer 2    */
/* Input params:       uiTimer2_Prescaler -> the prescaler       */
/*                                                setting value  */
/* Output params:      n/a                                       */
/* ***************************************************************/
void StepMotorCrtl::set_prescaler(TIMER2_PRESCALER uiTimer2_Prescaler){
  /*Set the clock source prescaler*/
  TCCR2B &= 0xf8;                  //- first clears the three LSB bits*
  TCCR2B |= uiTimer2_Prescaler;    //mask the register bits
  /*Saves the prescaler values*/
  switch (uiTimer2_Prescaler) {
    case TMR2_1_8_CLK:
      uiStored_Timer2_prescaler = 8;
      break;
    case TMR2_1_64_CLK:
      uiStored_Timer2_prescaler = 64;
      break;
    case TMR2_1_256_CLK:
      uiStored_Timer2_prescaler = 256;
      break;
    case TMR2_1_1024_CLK:
      uiStored_Timer2_prescaler = 1024;
      break;
  }
}
/* ****************************************************************/
/* Method name:        run_motor1                                 */
/* Method description: set the step motor 1 velocity  - connected */
/*                                                 to timer 0     */
/* Input params:       fRPM_motor1 -> the velocity value in RPM   */
/* Output params:      n/a                                        */
/* ****************************************************************/
void StepMotorCrtl::run_motor1(float fRPM_motor1){
  /*first disables all interrupts to avoid miss compare matchs*/
  cli();
  float ftimer_top, f_motor, f_step;
  if(fRPM_motor1 >=0.0){
     digitalWrite(ucMyDirPin1, HIGH);
  }else{
    fRPM_motor1 = -fRPM_motor1;
    digitalWrite(ucMyDirPin1, LOW);
  }
  /*Set the prescaler accordingly to the RPM speed, to obtain better speed resolution*/
  if (fRPM_motor1<=SPD_THRSHOLD_1024_PRESCALER) {
    set_prescaler(TMR0_1_1024_CLK);
  }else if(fRPM_motor1<=SPD_THRSHOLD_256_PRESCALER){
    set_prescaler(TMR0_1_256_CLK);
  }else if(fRPM_motor1<=SPD_THRSHOLD_64_PRESCALER){
    set_prescaler(TMR0_1_64_CLK);
  }else{
    set_prescaler(TMR0_1_8_CLK);
  }
  /*Convert RPM to Hz*/
  f_motor = fRPM_motor1/60.0;
  /*obtain the step frequency required to achieve the given motor speed (based on step motor resolution and microstep)*/
  f_step = f_motor*(float)N_STEP*(float)U_STEP;
  /*obtain the timer top match value to achieve the step frequency (based on the prescaler set and system clock speed)*/
  ftimer_top = ((float)SYSTEM_CLK/(2*(float)uiStored_Timer0_prescaler*f_step))-1.0;
  if(ftimer_top > (float)TMR0_2_MAX_COUNT)
    ftimer_top = (float)TMR0_2_MAX_COUNT;
  /*round the timer top value and update the timer match register*/
  OCR0A = round(ftimer_top);
  /*Once registers are updated, enables interrupt again*/
  sei();
}
/* ***************************************************************/
/* Method name:        run_motor2                                */
/* Method description: set the step motor 2 velocity - connected */
/*                                                 to timer 2    */
/* Input params:       fRPM_motor2 -> the velocity value in RPM  */
/* Output params:      n/a                                       */
/* ***************************************************************/
void StepMotorCrtl::run_motor2(float fRPM_motor2){
  /*first disables all interrupts to avoid miss compare matchs*/
  cli();
  float ftimer_top, f_motor, f_step;
  if(fRPM_motor2 >=0.0){
     digitalWrite(ucMyDirPin2, LOW);
  }else{
    fRPM_motor2 = -fRPM_motor2;
    digitalWrite(ucMyDirPin2, HIGH);
  }
  /*Set the prescaler accordingly to the RPM speed, to obtain better speed resolution*/
  if (fRPM_motor2<=SPD_THRSHOLD_1024_PRESCALER) {
    set_prescaler(TMR2_1_1024_CLK);
  }else if(fRPM_motor2<=SPD_THRSHOLD_256_PRESCALER){
    set_prescaler(TMR2_1_256_CLK);
  }else if(fRPM_motor2<=SPD_THRSHOLD_64_PRESCALER){
    set_prescaler(TMR2_1_64_CLK);
  }else{
    set_prescaler(TMR2_1_8_CLK);
  }
  /*Convert RPM to Hz*/
  f_motor = fRPM_motor2/60.0;
  /*obtain the step frequency required to achieve the given motor speed (based on step motor resolution and microstep)*/
  f_step = f_motor*(float)N_STEP*(float)U_STEP;
  /*obtain the timer top match value to achieve the step frequency (based on the prescaler set and system clock speed)*/
  ftimer_top = ((float)SYSTEM_CLK/(2*(float)uiStored_Timer2_prescaler*f_step))-1.0;
  if(ftimer_top > (float)TMR0_2_MAX_COUNT)
    ftimer_top = (float)TMR0_2_MAX_COUNT;
  /*round the timer top value and update the timer match register*/
  OCR2A = round(ftimer_top);
  /*Once registers are updated, enables interrupt again*/
  sei();
};
