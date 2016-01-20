/**
 * \class SoapDispenser boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _SOAPDISPENSER_H
#define _SOAPDISPENSER_H

#include "Protocol.h"

class SoapDispenser {
public:
	SoapDispenser();
	char * getOpenCommand(); ///< Returns the bytes used to open the soap dispenser
	char * getCloseCommand(); ///< Returns the bytes used to close the soap dispenser
	char * getStatusCommand(); ///< Returns the bytes used to get the status of the soap dispenser
};

#endif