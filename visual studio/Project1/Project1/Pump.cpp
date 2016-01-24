/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

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