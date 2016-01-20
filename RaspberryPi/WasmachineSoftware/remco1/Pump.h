// file Pump.h
#pragma once
#ifndef _PUMP_H
#define _PUMP_H

#include "Protocol.h"
#include <stdint.h>

class Pump {
public:
	Pump();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif