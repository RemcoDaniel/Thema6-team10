/**
 * \class WaterSensor boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _WATERSENSOR_H
#define _WATERSENSOR_H

#include "WaterController.h"

class WaterSensor {
public:
	WaterSensor();
	char * getWaterLevelCommand(); ///< Returns the bytes for the waterlevel 
};

#endif