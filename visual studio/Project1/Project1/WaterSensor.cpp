// file WaterSensor.cpp

#include "WaterSensor.h"

WaterSensor::WaterSensor() {}

char * WaterSensor::getWaterLevelCommand() {
	char command[3] = { WATER_LEVEL_REQ , STOP_CMD, '\0' };
	return command;
}