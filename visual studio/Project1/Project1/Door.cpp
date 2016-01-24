/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

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