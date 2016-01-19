/**
 * \class The Pump boundry class
 */

// file Pump.h
#pragma once
#ifndef _PUMP_H
#define _PUMP_H

#include "WaterController.h"

class Pump {
public:
	Pump();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif