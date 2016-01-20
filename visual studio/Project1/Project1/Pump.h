/**
 * \class Pump boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _PUMP_H
#define _PUMP_H

#include "WaterController.h"

class Pump {
public:
	Pump();
	char * getOnCommand(); ///< Returns the bytes for setting the waterpump on
	char * getOffCommand(); ///< Returns the bytes for setting the waterpump off
	char * getStatusCommand(); ///< Returns the bytes for getting the status. 
};

#endif