// file Pump.cpp

#include "Pump.h"

Pump::Pump() {}

char * Pump::getOnCommand() {
	return oncommand;
}

char * Pump::getOffCommand() {
	return offcommand;
}

char * Pump::getStatusCommand() {
	return statuscommand;
}