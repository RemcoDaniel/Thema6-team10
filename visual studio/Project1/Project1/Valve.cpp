/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

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