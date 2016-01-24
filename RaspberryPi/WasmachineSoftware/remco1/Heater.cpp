// file Heater.cpp

#include "Heater.h"

Heater::Heater() {}

char * Heater::getOnCommand() {
	return oncommand;
}

char * Heater::getOffCommand() {
	return offcommand;
}

char * Heater::getStatusCommand() {
	return statuscommand;
}