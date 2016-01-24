// file Pump.h
#pragma once
#ifndef _PUMP_H
#define _PUMP_H

#include "Protocol.h"
#include <stdint.h>

class Pump {
private:
	char oncommand[3] = { PUMP_REQ, ON_CMD, '\0' };
	char offcommand[3] = { PUMP_REQ, OFF_CMD, '\0' };
	char statuscommand[3] = { PUMP_REQ, STATUS_CMD, '\0' };

public:
	Pump();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif