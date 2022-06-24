/* ***************************************************************** */
/* File name:        Battery.cpp                                     */
/* File description: File dedicated to the hardware abstraction      */
/*                   layer related to the battery from the hardware  */
/*                   It is mainly used to verify the battery voltage */
/* Author name:      Grupo 2                                         */
/* Creation date:    23nov2021                                       */
/* Revision date:    05dez2021                                       */
/* ***************************************************************** */

#include "Battery.h"
#include "Arduino.h"

/* **********************************************************************/
/* Method name:        Battery                                       	  */
/* Method description: Battery class constructor					   	          */
/* Input params:       iBatteryPin -> the pin used by battery           */
/*                     fTargetTension -> the minimum tension desired    */
/*                     iLedPin -> the pin used by red LED               */
/* Output params:      n/a                                     			    */
/* **********************************************************************/
Battery::Battery(int iBatteryPin, float fTargetTension, int iLedPin){
    pinMode(iBatteryPin, INPUT);
	pinMode(iLedPin, OUTPUT);
	_iBatteryPin = iBatteryPin;
	_fTargetTension = fTargetTension;
	_iLedPin = iLedPin;
}

/* ************************************************* */
/* Method name:        check_battery                 */
/* Method description: verify if the battery voltage */
/*                     is within the appropriate     */
/*                     value (chosen by the group)   */
/*                     by the group                  */
/*                     true = LED turned off         */
/*                     false = LED turned on         */
/* Input params:       n/a                           */
/* Output params:      n/a                           */
/* ************************************************* */
void Battery::check_battery(){
	if(analogRead(_iBatteryPin)*5.0/1023.0 < _fTargetTension){
		digitalWrite(_iLedPin, HIGH);
	}
	else{
		digitalWrite(_iLedPin, LOW);
	}
}
