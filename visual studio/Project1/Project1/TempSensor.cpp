// file TempSensor.cpp

#include "TempSensor.h"

TempSensor::TempSensor() {}

char * TempSensor::getTempCommand() {
	char command[3] = { TEMPERATURE_REQ, STOP_CMD, '\0' };
	return command;
}