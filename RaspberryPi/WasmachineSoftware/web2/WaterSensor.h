// file WaterSensor.h
#pragma once
#ifndef _WATERSENSOR_H
#define _WATERSENSOR_H

#include "Protocol.h"
#include <stdint.h>

class WaterSensor {
private:
	char waterlevelcommand[3] = { WATER_LEVEL_REQ , UNDEFINED_CMD, '\0' };
public:
	WaterSensor();
	char * getWaterLevelCommand();
};

#endif