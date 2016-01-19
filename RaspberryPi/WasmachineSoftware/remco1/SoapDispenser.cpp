// file SoapDispenser.cpp

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