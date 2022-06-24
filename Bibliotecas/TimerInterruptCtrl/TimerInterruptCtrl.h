/* **********************************************************************/
/* File name:       TimerInterruptCrtl.h                                */
/* File description:  Internal timer configuration                      */
/*                    to operate an interuption                         */
/* Author name:      Grupo 2                                            */
/* Creation date:    31ago2021                                          */
/* Revision date:    05dez2021                                          */
/* **********************************************************************/


#ifndef TMR1_INTERRUPT_CRTL
#define TMR1_INTERRUPT_CRTL

// Definição dos pinos
#define SYSTEM_CLK  16000000  //system clock speed in Hz
#define TMR1_MAX_COUNT 65535U
#define PRTIM1_bit 3
#define COM1A0_bit 6
#define COM1A1_bit 7
#define TCCR1A_WGM10_bit 0
#define TCCR1A_WGM11_bit 1
#define TCCR1B_WGM12_bit 3
#define TCCR1B_WGM13_bit 4
#define TIMSK1_OCIE1A_bit 1
#define TIME_THRSHOLD_1024_PRESCALER 1.0486f
#define TIME_THRSHOLD_256_PRESCALER 0.2621f
#define TIME_THRSHOLD_64_PRESCALER 0.0328f
#define TIME_THRSHOLD_8_PRESCALER 0.004096f

enum TIMER1_PRESCALER {TMR1_1_CLK = 0x01, TMR1_1_8_CLK = 0x02, TMR1_1_64_CLK = 0x03, TMR1_1_256_CLK = 0x04, TMR1_1_1024_CLK = 0x05};

class TimerInterruptCtrl
{
	public:


		/* **********************************************************************/
		/* Method name:        TimerInterruptCtrl                               */
		/* Method description: Contructor method: instance of the object        */
		/* Input params:       n/a                                              */
		/* Output params:      n/a                                              */
		/* **********************************************************************/
		TimerInterruptCtrl();
		/* **********************************************************************/
		/* Method name:        TimerInterrupt_init                              */
		/* Method description: Initialize and configure the internal            */
		/* 16 bits timer1 to fire a interruption on OCR1A top value match       */
		/* Input params:       unsigned long ul_periodMicrosecs: timer interrupt*/
		/*                     period in microseconds                           */
		/* Output params:      n/a                                              */
		/* **********************************************************************/
		void TimerInterrupt_init(unsigned long ul_periodMicrosecs);
		/* ****************************************************************/
		/* Method name:        set_prescaler                              */
		/* Method description: Set the clock source prescaler             */
		/* Input params:       uiTimer1_Prescaler: timer desired prescaler*/
		/* Output params:      n/a                                        */
		/* ****************************************************************/
		void set_prescaler(TIMER1_PRESCALER uiTimer1_Prescaler);

	private:
		 //saves the current timer1 set prescaler
		int iStored_Timer1_prescaler;
};

#endif
