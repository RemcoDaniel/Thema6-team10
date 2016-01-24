/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

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