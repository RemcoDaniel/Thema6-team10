// file Motor.cpp

#include "Motor.h"

Motor::Motor(UART uart) : uart(uart){
	speed = 0;
}

void Motor::setMotorSpeed(int speed) {		// getal van 0 - 1600 rpm
	speed = speed;
}

int Motor::getMotorSpeed() {		// getal van 0 - 1600 rpm
	uart.sendCommand(0x09, 0x00);
	// wachten op antwoord!!!
	return speed;
}

void Motor::turn(bool dir, int speed) {
	unsigned int hulp = speed / 25;		// speed kan van 0 - 1600 rpm.    stappen van 25, dus delen door 25 levert 0x00 - 0x40
	if (dir) {		// 1 = left
		hulp = hulp | 0x80;			// de RPM OR'en met 0x80
	}

	uart.sendCommand(0x0A, hulp);
}