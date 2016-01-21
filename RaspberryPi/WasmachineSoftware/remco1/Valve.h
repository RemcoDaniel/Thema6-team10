// file Valve.h
#pragma once
#ifndef _VALVE_H
#define _VALVE_H

#include "Protocol.h"
#include <stdint.h>

class Valve {
private:
	char oncommand[3] = { WATER_VALVE_REQ, ON_CMD, '\0' };
	char offcommand[3] = { WATER_VALVE_REQ, OFF_CMD, '\0' };
	char statuscommand[3] = { WATER_VALVE_REQ, STATUS_CMD, '\0' };

public:
	Valve();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif