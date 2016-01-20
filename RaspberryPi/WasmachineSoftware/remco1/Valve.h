// file Valve.h
#pragma once
#ifndef _VALVE_H
#define _VALVE_H

#include "Protocol.h"
#include <stdint.h>

class Valve {
public:
	Valve();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif