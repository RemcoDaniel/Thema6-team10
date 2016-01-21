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
	char * getTempCommand();
};

#endif