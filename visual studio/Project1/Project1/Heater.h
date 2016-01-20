/**
 * \class Heater boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _HEATER_H
#define _HEATER_H

#include "Protocol.h"

class Heater {
public:
	Heater();
	char * getOnCommand();
	/*! \fn char * getOnCommand()
	 *	\brief Returns the bytes to set the heater on
	 *	\return a char pointer to a 2 char array
	 */
	char * getOffCommand();
	/*! \fn char * getOffCommand()
	 *	\brief Returns the bytes to set the heater off
	 *	\return a char pointer to a 2 char array
	 */
	char * getStatusCommand();
	/*! \fn char * getStatusCommand()
	 *	\brief Returns the bytes to get the status of the heater
	 *	\return a char pointer to a 2 char array
	 */
};

#endif