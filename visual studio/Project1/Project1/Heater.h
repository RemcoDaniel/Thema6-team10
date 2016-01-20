/**
 * \class Heater boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _HEATER_H
#define _HEATER_H

#include "Protocol.h"

class Heater {
public:
	Heater();
	char * getOnCommand(); ///< Returns the bytes to set the heater on
	char * getOffCommand(); ///< Returns the bytes to set the heater offf.
	char * getStatusCommand();  ///< Returns the bytes to get the status of the heater
};

#endif