/* ******************************************************************* */
/* File name:        Warning.h                                         */
/* File description: Header file containing the functions/methods      */
/*                   interfaces for handling the warning (green LED)   */
/*                   from the hardware kit                             */
/*																	   																 */
/* Author name:      Grupo 2 - André M., Gabriel K., Gidiones N., Ru S.*/
/* Creation date:    23nov2021                                         */
/* Revision date:    05dez2021                                         */
/* ******************************************************************* */

#ifndef WARNING_H
#define WARNING_H

#define MAX_ANGLE 30.0f

class Warning
{
	public:
    /* ********************************************************************/
    /* Method name:        Warning                                       	*/
    /* Method description: Warning class constructor					   					*/
    /* Input params:       iLedPIn -> the pin used by the green LED      	*/
    /* Output params:      n/a                                     				*/
    /* ********************************************************************/
		Warning(int iLedPin);

		/* ***************************************************/
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
		void check_angle(float fAngle);

	private:
		int iGreenLedPin;
};
#endif
