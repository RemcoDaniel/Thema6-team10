// file WaterSensor.h
#pragma once
#ifndef _WATERSENSOR_H
#define _WATERSENSOR_H

#include "WaterController.h"

class WaterSensor {
public:
	WaterSensor();
	char * getWaterLevelCommand();
};

#endif