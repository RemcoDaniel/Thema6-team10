// file Heater.cpp

#include "Heater.h"

Heater::Heater() {}

char * Heater::getOnCommand() {
	char command[3] = { HEATING_UNIT_REQ, ON_CMD, '\0' };
	return command;
}

char * Heater::getOffCommand() {
	char command[3] = { HEATING_UNIT_REQ, OFF_CMD, '\0' };
	return command;
}

char * Heater::getStatusCommand() {
	char command[3] = { HEATING_UNIT_REQ, STATUS_CMD, '\0' };
	return command;
}