/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

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