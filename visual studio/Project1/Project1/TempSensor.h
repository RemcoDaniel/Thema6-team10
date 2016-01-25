// file TempSensor.h
#pragma once
#ifndef _TEMPSENSOR_H
#define _TEMPSENSOR_H

#include "Protocol.h"
#include <stdint.h>

class TempSensor {
private:
	char tempcommand[3] = { TEMPERATURE_REQ, UNDEFINED_CMD , '\0' };
public:
	TempSensor();
	/*! \fn char * getTempCommand()
	*	\brief Returns the bytes to get the temprature
	*	\return a char pointer to a 2 char array
	*/
	char * getTempCommand();
};

#endif
