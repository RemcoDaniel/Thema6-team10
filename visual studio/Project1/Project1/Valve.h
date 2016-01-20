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
	char * getOnCommand();
	/*! \fn char * getOnCommand()
	 *	\brief Returns the bytes to set the valve open
	 *	\return a char pointer to a 2 char array
	 */
	char * getOffCommand();
	/*! \fn char * getOffCommand()
	 *	\brief Returns the bytes to set the valve close
	 *	\return a char pointer to a 2 char array
	 */
	char * getStatusCommand();
	/*! \fn char * getStatusCommand()
	 *	\brief Returns the bytes to get the status of the valve
	 *	\return a char pointer to a 2 char array
	 */
};

#endif