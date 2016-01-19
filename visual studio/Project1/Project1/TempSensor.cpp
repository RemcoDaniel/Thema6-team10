// file TempSensor.cpp

#include "TempSensor.h"

TempSensor::TempSensor() {}

char * TempSensor::getTempCommand() {
	char command[3] = { TEMPERATURE_REQ, '\0' };
	return command;
}