/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

#include "Door.h"

Door::Door() {}

char * Door::getLockCommand() {
	char command[3] = { DOOR_LOCK_REQ, LOCK_CMD, '\0' };
	return command;
}

char * Door::getUnlockCommand() {
	char command[3] = { DOOR_LOCK_REQ, UNLOCK_CMD, '\0' };
	return command;
}

char * Door::getStatusCommand() {
	char command[3] = { DOOR_LOCK_REQ, STATUS_CMD, '\0' };
	return command;
}