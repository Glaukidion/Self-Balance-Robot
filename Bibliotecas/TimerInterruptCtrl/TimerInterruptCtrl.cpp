/* **********************************************************************/
/* File name:       TimerInterruptCrtl.h                                */
/* File description:  Internal timer configuration                      */
/*                    to operate an interuption                         */
/* Author name:      Grupo 2                                            */
/* Creation date:    31ago2021                                          */
/* Revision date:    05dez2021                                          */
/* **********************************************************************/

#include "TimerInterruptCtrl.h"
#include <Arduino.h>


/* **********************************************************************/
/* Method name:        TimerInterruptCtrl                               */
/* Method description: Empty Contructor method: instance of the object  */
/* Input params:       n/a                                              */
/* Output params:      n/a                                              */
/* **********************************************************************/
TimerInterruptCtrl::TimerInterruptCtrl(){

};
/* **********************************************************************/
/* Method name:        TimerInterrupt_init                              */
/* Method description: Initialize and configure the internal timer 1    */
/*                     to fire timed interruption                       */
/* Input params:       unsigned long ul_periodMicrosecs: timer interrupt*/
/*                     period in microseconds                           */
/* Output params:      n/a                                              */
/* **********************************************************************/
void TimerInterruptCtrl::TimerInterrupt_init(unsigned long ul_periodMicrosecs){
  float f_ISR, f_top, f_ISRperiod_secs;
  f_ISRperiod_secs = (float)ul_periodMicrosecs/1000000.0f;
  /*8 bit Timer/Counter 0 initialization*/
  PRR &= ~(1 << PRTIM1_bit);            //Disable power reduition at Timer 1
  /*Set the timer 1 to operate at Fast - PWM mode - with TOP OCR1A register*/
  //disconnect the OC1x pins
  TCCR1A &= ~(1 << COM1A0_bit);
  TCCR1A &= ~(1 << COM1A1_bit);
  //set the Fast-PWM mode
  TCCR1A |= 1 << TCCR1A_WGM10_bit;
  TCCR1A |= 1 << TCCR1A_WGM11_bit;
  TCCR1B |= 1 << TCCR1B_WGM12_bit;
  TCCR1B |= 1 << TCCR1B_WGM13_bit;
  /*Set the prescaler accordingly to the interrupt period, to obtain better time resolution*/
  if (f_ISRperiod_secs>TIME_THRSHOLD_1024_PRESCALER) {
    set_prescaler(TMR1_1_1024_CLK);
  }else if(f_ISRperiod_secs>TIME_THRSHOLD_256_PRESCALER){
    set_prescaler(TMR1_1_256_CLK);
  }else if(f_ISRperiod_secs>TIME_THRSHOLD_64_PRESCALER){
    set_prescaler(TMR1_1_64_CLK);
  }else if(f_ISRperiod_secs>TIME_THRSHOLD_8_PRESCALER){
    set_prescaler(TMR1_1_8_CLK);
  }else{
    set_prescaler(TMR1_1_CLK);
  }
  /*Calculate the timer 1 top value based on the interrupt period*/
  f_ISR = 1.0/f_ISRperiod_secs;
  f_top = ((float)SYSTEM_CLK/((float)iStored_Timer1_prescaler*f_ISR))-1.0;
  if(f_top > TMR1_MAX_COUNT){
    f_top = (float)TMR1_MAX_COUNT;
  }
  OCR1A = round (f_top);
  /*Enables interrupt(globally)*/
  sei();
  /*Enables the interrupt compare match A*/
  TIMSK1 |= 1 << TIMSK1_OCIE1A_bit;
}

/* ****************************************************************/
/* Method name:        set_prescaler                              */
/* Method description: Set the clock source prescaler             */
/* Input params:       uiTimer1_Prescaler: timer desired prescaler*/
/* Output params:      n/a                                        */
/* ****************************************************************/
void TimerInterruptCtrl::set_prescaler(TIMER1_PRESCALER uiTimer1_Prescaler){
  /*Set the clock source prescaler*/
  TCCR1B &= 0xf8;                  //- first clears the three LSB bits*
  TCCR1B |= uiTimer1_Prescaler;         //mask the register bits
  /*Saves the prescaler values*/
  switch (uiTimer1_Prescaler) {
    case TMR1_1_CLK:
      iStored_Timer1_prescaler = 1;
      break;
    case TMR1_1_8_CLK:
      iStored_Timer1_prescaler = 8;
      break;
    case TMR1_1_64_CLK:
      iStored_Timer1_prescaler = 64;
      break;
    case TMR1_1_256_CLK:
      iStored_Timer1_prescaler = 256;
      break;
    case TMR1_1_1024_CLK:
      iStored_Timer1_prescaler = 1024;
      break;
  }
}
