// file SoapDispenser.h

#ifndef _SOAPDISPENSER_H
#define _SOAPDISPENSER_H

#include "Protocol.h"
#include <stdint.h>

class SoapDispenser {
private:
	char opencommand[3] = { SOAP_DISPENSER_REQ, OPEN_CMD, '\0' };
	char closecommand[3] = { SOAP_DISPENSER_REQ, CLOSE_CMD, '\0' };
	char statuscommand[3] = { SOAP_DISPENSER_REQ, LOCK_CMD, '\0' };
	
public:
	SoapDispenser();
	char * getOpenCommand();
	char * getCloseCommand();
	char * getStatusCommand();
};

#endif