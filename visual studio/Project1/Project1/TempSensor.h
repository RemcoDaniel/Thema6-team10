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

class TempSensor {
public:
	TempSensor();
	char * getTempCommand(); ///< Returns the bytes used to get the temprature from the temprature sensor
};

#endif