// file Door.cpp

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