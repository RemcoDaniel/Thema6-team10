/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

#include "Motor.h"
#include "Protocol.h"

Motor::Motor() {}

char * Motor::getMotorSpeedCommand() {		// getal van 0 - 1600 rpm
	char command[3] = { GET_RPM_REQ, '\0' };
	return command;
}

char * Motor::turn(bool dir, int speed) {
	unsigned int hulp = speed / 25;		// speed kan van 0 - 1600 rpm.    stappen van 25, dus delen door 25 levert 0x00 - 0x40
	if (dir) {		// 1 = left
		hulp = hulp | 0x80;			// de RPM OR'en met 0x80
	}
	char command[3] = { SET_RPM_REQ, hulp, '\0' };
	return command;
}