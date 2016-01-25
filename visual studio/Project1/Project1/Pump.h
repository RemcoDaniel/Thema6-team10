// file Pump.h
#pragma once
#ifndef _PUMP_H
#define _PUMP_H

#include "Protocol.h"
#include <stdint.h>

class Pump {
private:
	char oncommand[3] = { PUMP_REQ, ON_CMD, '\0' };
	char offcommand[3] = { PUMP_REQ, OFF_CMD, '\0' };
	char statuscommand[3] = { PUMP_REQ, STATUS_CMD, '\0' };

public:
	Pump();
	/*! \fn char * getOnCommand()
	*	\brief Returns the bytes to set the valve open
	*	\return a char pointer to a 2 char array
	*/
	char * getOnCommand();
	/*! \fn char * getOffCommand()
	*	\brief Returns the bytes to set the valve open
	*	\return a char pointer to a 2 char array
	*/
	char * getOffCommand();
	/*! \fn char * getStatusCommand()
	*	\brief Returns the bytes to set the valve open
	*	\return a char pointer to a 2 char array
	*/
	char * getStatusCommand();
};

#endif
