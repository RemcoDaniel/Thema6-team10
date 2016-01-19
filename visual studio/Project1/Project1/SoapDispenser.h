/**
 * \class The SoapDispenser boundry class
 */

#ifndef _SOAPDISPENSER_H
#define _SOAPDISPENSER_H

#include "Protocol.h"

class SoapDispenser {
public:
	SoapDispenser();
	char * getOpenCommand();
	char * getCloseCommand();
	char * getStatusCommand();
};

#endif