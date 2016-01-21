// file SoapDispenser.cpp

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