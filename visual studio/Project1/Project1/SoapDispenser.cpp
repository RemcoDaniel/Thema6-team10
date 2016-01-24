/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

#include "SoapDispenser.h"

SoapDispenser::SoapDispenser() {}

char * SoapDispenser::getOpenCommand() {
	return opencommand;
}

char * SoapDispenser::getCloseCommand() {
	return closecommand;
}

char * SoapDispenser::getStatusCommand() {
	return statuscommand;
}