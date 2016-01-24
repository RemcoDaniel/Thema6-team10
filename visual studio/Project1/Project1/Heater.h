// file Heater.h
#pragma once
#ifndef _HEATER_H
#define _HEATER_H

#include "Protocol.h"
#include <stdint.h>

class Heater {
private:
	char oncommand[3] = { HEATING_UNIT_REQ, ON_CMD, '\0' };
	char offcommand[3] = { HEATING_UNIT_REQ, OFF_CMD, '\0' };
	char statuscommand[3] = { HEATING_UNIT_REQ, STATUS_CMD, '\0' };


public:
	Heater();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif