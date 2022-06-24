/* ********************************************************************/
/* File name:       StepMotorCrtl.h                                   */
/* File description: header file for the step motor control	- Timer   */
/* 						 configuration to operate at controlled PWM frequency   */
/* Author name:      Grupo 2                                          */
/* Creation date:    31ago2021                                        */
/* Revision date:    05dez2021                                        */
/* ********************************************************************/

#ifndef STEP_MOTOR_CTRL_H
#define STEP_MOTOR_CTRL_H

#define N_STEP 200						//number of step per turn
#define U_STEP 16							//number of microsteps
#define SYSTEM_CLK  16000000  //system clock speed in Hz
#define TMR0_2_MAX_COUNT 255U	//Counter maximum value
#define PRTIM0_bit 5
#define PRTIM2_bit 6
#define COM0_2A1_bit 7
#define COM0_2A0_bit 6
#define TCCR0_2A_WGM00_bit 0
#define TCCR0_2A_WGM01_bit 1
#define TCCR0_2B_WGM02_bit 3
#define SPD_THRSHOLD_1024_PRESCALER 2.289f
#define SPD_THRSHOLD_256_PRESCALER 9.155f
#define SPD_THRSHOLD_64_PRESCALER 73.24f

//defines the prescaler option for timer 0 and timer 2
enum TIMER0_PRESCALER {TMR0_1_8_CLK = 0x02, TMR0_1_64_CLK = 0x03, TMR0_1_256_CLK = 0x04, TMR0_1_1024_CLK = 0x05};
enum TIMER2_PRESCALER {TMR2_1_8_CLK = 0x02, TMR2_1_64_CLK = 0x04, TMR2_1_256_CLK = 0x06, TMR2_1_1024_CLK = 0x07};

class StepMotorCrtl
{
	public:
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
		StepMotorCrtl(unsigned char ucStepPin1, unsigned char ucStepPin2, unsigned char ucDirPin1, unsigned char ucDirPin2,
			unsigned char ucEnablePin1, unsigned char ucEnablePin2);

		/* ***************************************************************/
		/* Method name:        step_motors_init                          */
		/* Method description: Initialize the counters used by the motors*/
		/* Input params:       n/a                                       */
		/* Output params:      n/a                                       */
		/* ***************************************************************/
		void step_motors_init();
		/* ****************************************************************/
		/* Method name:        run_motor1                                 */
		/* Method description: set the step motor 1 velocity  - connected */
		/*                                                 to timer 0     */
		/* Input params:       fRPM_motor1 -> the velocity value in RPM   */
		/* Output params:      n/a                                        */
		/* ****************************************************************/
		void run_motor1(float fRPM_motor1);
		/* ***************************************************************/
		/* Method name:        run_motor2                                */
		/* Method description: set the step motor 2 velocity - connected */
		/*                                                 to timer 2    */
		/* Input params:       fRPM_motor2 -> the velocity value in RPM  */
		/* Output params:      n/a                                       */
		/* ***************************************************************/
		void run_motor2(float fRPM_motor2);
		/* ***************************************************************/
		/* Method name:        set_prescaler                             */
		/* Method description: set the prescalers used by the timer 0    */
		/* Input params:       uiTimer0_Prescaler -> the prescaler       */
		/*                                                setting value  */
		/* Output params:      n/a                                       */
		/* ***************************************************************/
		void set_prescaler(TIMER0_PRESCALER uiTimer0_Prescaler);
		/* ***************************************************************/
		/* Method name:        set_prescaler                             */
		/* Method description: set the prescalers used by the timer 2    */
		/* Input params:       uiTimer2_Prescaler -> the prescaler       */
		/*                                                setting value  */
		/* Output params:      n/a                                       */
		/* ***************************************************************/
		void set_prescaler(TIMER2_PRESCALER uiTimer2_Prescaler);

	private:
		//internal variables
		unsigned char ucMyStepPin1;
		unsigned char ucMyStepPin2;
		unsigned char ucMyDirPin1;
		unsigned char ucMyDirPin2;
		unsigned char ucMyEnablePin1;
		unsigned char ucMyEnablePin2;
		unsigned int uiStored_Timer0_prescaler;
		unsigned int uiStored_Timer2_prescaler;
};

#endif
