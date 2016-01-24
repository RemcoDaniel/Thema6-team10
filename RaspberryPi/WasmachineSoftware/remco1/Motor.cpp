// file Motor.cpp

#include "Motor.h"

Motor::Motor() {}

char * Motor::getMotorSpeedCommand() {		// getal van 0 - 1600 rpm
	return motorspeedcommand;
}

char * Motor::turn(bool dir, int speed) {
	int hulp = 0;
	if (speed != 0) {
		hulp = speed / 25;		// speed kan van 0 - 1600 rpm.    stappen van 25, dus delen door 25 levert 0x00 - 0x40
	}
	if (dir) {		// 1 = left
		hulp = hulp | 0x80;			// de RPM OR'en met 0x80
	}
	turncommand[1] = hulp;
	return turncommand;
}