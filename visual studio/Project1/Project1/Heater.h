/**
 * \class The Heater boundry class
 */

#ifndef _HEATER_H
#define _HEATER_H

#include "Protocol.h"

class Heater {
public:
	Heater();
	char * getOnCommand(); ///< Used to get the 2 bytes for the UART that mean "turn the Heater on"
	char * getOffCommand(); ///< Used to get the 2 bytes for the UART that mean "turn the Heater off"
	char * getStatusCommand(); ///< Used to get the 2 bytes for the UART that mean "is the Heater on or not" 
};

#endif