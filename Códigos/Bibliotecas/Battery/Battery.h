/* ***************************************************************** */
/* File name:        Battery.h                                       */
/* File description: Header file containing the functions/methods    */
/*                   interfaces for handling the battery from the    */
/*                   hardware kit                                    */
/* Author name:      Grupo 2                                         */
/* Creation date:    23nov2021                                       */
/* Revision date:    05dez2021                                       */
/* ***************************************************************** */

#ifndef BATTERY_H
#define BATTERY_H

#include "Arduino.h"

class Battery
{
	public:
	    /* **********************************************************************/
        /* Method name:        Battery                                       	*/
        /* Method description: Battery class constructor					   					*/
        /* Input params:       iBatteryPin -> the pin used by battery         */
        /*                     fTargetTension -> the minimum tension desired  */
        /*                     iLedPin -> the pin used by red LED             */
        /* Output params:      n/a                                     				*/
        /* ********************************************************************/
		Battery(int iBatteryPin, float fTargetTension, int iLedPin);

		/* *******************************************************/
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
		void check_battery();

	private:
		int _iLedPin;
		int _iBatteryPin;
		float _fTargetTension;
};
#endif
