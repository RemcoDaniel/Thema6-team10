/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

#include "SoapDispenser.h"

SoapDispenser::SoapDispenser() {}

char * SoapDispenser::getOpenCommand() {
	char command[3] = { SOAP_DISPENSER_REQ, OPEN_CMD, '\0' };
	return command;
}

char * SoapDispenser::getCloseCommand() {
	char command[3] = { SOAP_DISPENSER_REQ, CLOSE_CMD, '\0' };
	return command;
}

char * SoapDispenser::getStatusCommand() {
	char command[3] = { SOAP_DISPENSER_REQ, LOCK_CMD, '\0' };
	return command;
}