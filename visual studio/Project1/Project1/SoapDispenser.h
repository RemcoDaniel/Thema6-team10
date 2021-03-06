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
	/*! \fn char * getOpenCommand()
	*	\brief Returns the bytes to open the dispenser
	*	\return a char pointer to a 2 char array
	*/
	char * getOpenCommand();
	/*! \fn char * getCloseCommand()
	*	\brief Returns the bytes to close the dispenser
	*	\return a char pointer to a 2 char array
	*/
	char * getCloseCommand();
	/*! \fn char * getStatusCommand()
	*	\brief Returns the bytes to get the status of the dispenser
	*	\return a char pointer to a 2 char array
	*/
	char * getStatusCommand();
};

#endif
