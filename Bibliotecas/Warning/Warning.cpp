/* ******************************************************************* */
/* File name:        Warning.cpp                                       */
/* File description: File dedicated to the hardware abstraction        */
/*                   layer related to the warning (green LED)          */
/*                   from the hardware.                                */
/*                   It is mainly used to verify when the robot is     */
/*                   fallen.                                           */
/*																	                                   */
/* Author name:      Grupo 2 - André M., Gabriel K., Gidiones N., Ru S.*/
/* Creation date:    23nov2021                                         */
/* Revision date:    05dez2021                                         */
/* ******************************************************************* */

#include "Warning.h"
#include <Arduino.h>

/* **********************************************************************/
/* Method name:        Warning                                       	  */
/* Method description: Warning class constructor					   	          */
/* Input params:       iLedPIn -> the pin used by the green LED      	  */
/* Output params:      n/a                                     			    */
/* **********************************************************************/
Warning::Warning(int iLedPin){
  pinMode(iLedPin, OUTPUT);
	iGreenLedPin = iLedPin;
}
/* ************************************************* */
/* Method name:        check_angle                   */
/* Method description: verify if the absolute value  */
/*                     of the instantaneous angle    */
/*                     is less than the max (chosen) */
/*                     true = LED turned off         */
/*                     false = LED turned on         */
/* Input params:       fAngle: the instantaneous     */
/*                             angle                 */
/* Output params:      n/a                           */
/* ************************************************* */
void Warning::check_angle(float fAngle){
	if(fabs(fAngle) > MAX_ANGLE){
		digitalWrite(iGreenLedPin, HIGH);
	}else{
		digitalWrite(iGreenLedPin, LOW);
	}
}
