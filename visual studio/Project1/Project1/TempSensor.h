/**
 * \class The TempSensor boundry class
 */
 
#pragma once
#ifndef _TEMPSENSOR_H
#define _TEMPSENSOR_H

#include "Protocol.h"

class TempSensor {
public:
	TempSensor();
	char * getTempCommand();
};

#endif