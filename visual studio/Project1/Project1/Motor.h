// file Motor.h
#pragma once
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
	char * getMotorSpeedCommand();				// getal van 0 - 1600 rpm
	char * turn(bool dir, int speed);			// direction: 0 = right, 1 = left
};

#endif