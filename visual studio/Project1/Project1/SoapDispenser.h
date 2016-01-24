/**
 * \class SoapDispenser boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

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
	char * getOpenCommand(); ///< Returns the bytes used to open the soap dispenser
	char * getCloseCommand(); ///< Returns the bytes used to close the soap dispenser
	char * getStatusCommand(); ///< Returns the bytes used to get the status of the soap dispenser
};

#endif