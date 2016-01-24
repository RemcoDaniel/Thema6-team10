/**
 * \class TempSensor boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */
 
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
	char * getTempCommand(); ///< Returns the bytes used to get the temprature from the temprature sensor
};

#endif