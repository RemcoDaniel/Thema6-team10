/**
 * \class Pump boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _PUMP_H
#define _PUMP_H

#include "WaterController.h"

class Pump {
public:
	Pump();
	char * getOnCommand();
	/*! \fn char * getOnCommand()
	 *	\brief Returns the bytes to set the pump on
	 *	\return a char pointer to a 2 char array
	 */
	char * getOffCommand();
	/*! \fn char * getOffCommand()
	 *	\brief Returns the bytes to set the pump off
	 *	\return a char pointer to a 2 char array
	 */
	char * getStatusCommand();
	/*! \fn char * getStatusCommand()
	 *	\brief Returns the bytes to get the status of the pump
	 *	\return a char pointer to a 2 char array
	 */
};

#endif