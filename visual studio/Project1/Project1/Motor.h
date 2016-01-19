/**
 * \class The Motor boundry class
 */

#pragma once
#ifndef _MOTOR_H
#define _MOTOR_H

class Motor {
public:
	Motor();
	char * getMotorSpeedCommand();				// getal van 0 - 1600 rpm
	char * turn(bool dir, int speed);			// direction: 0 = right, 1 = left
};

#endif