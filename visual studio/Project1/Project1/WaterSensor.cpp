/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

#include "WaterSensor.h"

WaterSensor::WaterSensor() {}

char * WaterSensor::getWaterLevelCommand() {
	return waterlevelcommand;
}