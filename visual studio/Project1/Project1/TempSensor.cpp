/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

#include "TempSensor.h"

TempSensor::TempSensor() {}

char * TempSensor::getTempCommand() {
	char command[3] = { TEMPERATURE_REQ, '\0' };
	return command;
}