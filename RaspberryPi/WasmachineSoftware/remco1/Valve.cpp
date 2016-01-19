// file Valve.cpp

#include "Valve.h"

Valve::Valve() {}

char * Valve::getOnCommand() {
	char command[3] = { WATER_VALVE_REQ, ON_CMD, '\0' };
	return command;
}

char * Valve::getOffCommand() {
	char command[3] = { WATER_VALVE_REQ, OFF_CMD, '\0' };
	return command;
}

char * Valve::getStatusCommand() {
	char command[3] = { WATER_VALVE_REQ, STATUS_CMD, '\0' };
	return command;
}