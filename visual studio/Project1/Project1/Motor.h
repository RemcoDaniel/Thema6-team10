/**
 * \class Motor boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _MOTOR_H
#define _MOTOR_H

#include "Protocol.h"
#include <stdint.h>
#include <iostream>

class Motor {
private:
	char motorspeedcommand[3] = { GET_RPM_REQ, STOP_CMD, '\0' };
	char turncommand[3] = { SET_RPM_REQ, 0, '\0' };

public:
	Motor();
	
	/*! \fn char * getMotorSpeedCommand()
	 *	\brief Returns the bytes to get the speed of the motor
	 *	\return a char pointer to a 2 char array
	 */
	char * getMotorSpeedCommand();
	
	/*! \fn char * turn(bool dir, int speed)
	 *	\brief Sets the right speed and the right direction (or left direction) of the motor
	 *	\param dir the direction of the motor. false = right, true = left
	 *	\param speed the speed of the rpm. 0 <= x <= 1600
	 *	\return the bytes used to set the RPM in a direction.
	 */
	char * turn(bool dir, int speed);
};

#endif