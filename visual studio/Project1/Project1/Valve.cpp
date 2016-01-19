/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

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