// file Door.cpp

#include "Door.h"

Door::Door() {}

char * Door::getLockCommand() {
	return lockcommand;
}

char * Door::getUnlockCommand() {
	return unlockcommand;
}

char * Door::getStatusCommand() {
	return statuscommand;
}