/**
 * \class Valve boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */
 
// file Valve.h
#pragma once
#ifndef _VALVE_H
#define _VALVE_H

#include "WaterController.h"

class Valve {
public:
	Valve();
	char * getOnCommand(); ///< Returns the bytes for turning the water valve on
	char * getOffCommand(); ///< Returns the bytes for turning the water valve off
	char * getStatusCommand(); ///< Returns the bytes for getting the status of the water valve
};

#endif