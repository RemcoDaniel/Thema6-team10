/**
 * \class Heater boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _HEATER_H
#define _HEATER_H

#include "Protocol.h"
#include <stdint.h>

class Heater {
private:
	char oncommand[3] = { HEATING_UNIT_REQ, ON_CMD, '\0' };
	char offcommand[3] = { HEATING_UNIT_REQ, OFF_CMD, '\0' };
	char statuscommand[3] = { HEATING_UNIT_REQ, STATUS_CMD, '\0' };


public:
	Heater();
	
	/*! \fn char * getOnCommand()
	 *	\brief Returns the bytes to set the heater on
	 *	\return a char pointer to a 2 char array
	 */
	char * getOnCommand();
	
	/*! \fn char * getOffCommand()
	 *	\brief Returns the bytes to set the heater off
	 *	\return a char pointer to a 2 char array
	 */
	char * getOffCommand();
	
	/*! \fn char * getStatusCommand()
	 *	\brief Returns the bytes to get the status of the heater
	 *	\return a char pointer to a 2 char array
	 */
	char * getStatusCommand();
};

#endif