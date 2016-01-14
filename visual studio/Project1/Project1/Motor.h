// file Motor.h

#ifndef _MOTOR_H
#define _MOTOR_H

#include "MotorController.h"
#include "UART.h"

class Motor {
private:
	int speed;
	UART &uart;
public:
	Motor(UART uart);
	void setMotorSpeed(int speed);		// getal van 0 - 1600 rpm    // overbodig?
	int getMotorSpeed();				// getal van 0 - 1600 rpm
	void turn(bool dir, int speed);	//direction: 0 = right, 1 = left
};

#endif