// file Pump.cpp

#include "Pump.h"

Pump::Pump() {}

char * Pump::getOnCommand() {
	char command[3] = { PUMP_REQ, ON_CMD, '\0' };
	return command;
}

char * Pump::getOffCommand() {
	char command[3] = { PUMP_REQ, OFF_CMD, '\0' };
	return command;
}

char * Pump::getStatusCommand() {
	char command[3] = { PUMP_REQ, STATUS_CMD, '\0' };
	return command;
}