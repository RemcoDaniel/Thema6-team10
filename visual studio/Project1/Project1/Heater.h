// file Heater.h

#ifndef _HEATER_H
#define _HEATER_H

#include "Protocol.h"

class Heater {
public:
	Heater();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif