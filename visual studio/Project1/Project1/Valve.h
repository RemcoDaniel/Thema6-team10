// file Valve.h
#pragma once
#ifndef _VALVE_H
#define _VALVE_H

#include "Protocol.h"
#include <stdint.h>

class Valve {
private:
	char oncommand[3] = { WATER_VALVE_REQ, ON_CMD, '\0' };
	char offcommand[3] = { WATER_VALVE_REQ, OFF_CMD, '\0' };
	char statuscommand[3] = { WATER_VALVE_REQ, STATUS_CMD, '\0' };

public:
	Valve();
	/*! \fn char * getOnCommand()
	 *	\brief Returns the bytes to set the valve open
	 *	\return a char pointer to a 2 char array
	 */
	char * getOnCommand();
	/*! \fn char * getOffCommand()
	 *	\brief Returns the bytes to set the valve close
	 *	\return a char pointer to a 2 char array
	 */
	char * getOffCommand();
	/*! \fn char * getStatusCommand()
	 *	\brief Returns the bytes to get the status of the valve
	 *	\return a char pointer to a 2 char array
	 */
	char * getStatusCommand();
};

#endif