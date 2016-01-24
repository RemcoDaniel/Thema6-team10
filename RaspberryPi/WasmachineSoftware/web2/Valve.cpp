// file Valve.cpp

#include "Valve.h"

Valve::Valve() {}

char * Valve::getOnCommand() {
	return oncommand;
}

char * Valve::getOffCommand() {
	return offcommand;
}

char * Valve::getStatusCommand() {
	return statuscommand;
}