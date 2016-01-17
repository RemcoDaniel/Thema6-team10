// file Valve.h

#ifndef _VALVE_H
#define _VALVE_H

#include "WaterController.h"

class Valve {
public:
	Valve();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif