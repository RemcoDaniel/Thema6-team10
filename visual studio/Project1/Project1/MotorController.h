// file MotorController.h

#ifndef _MOTORCONTROLLER_H
#define _MOTORCONTROLLER_H

#include "Motor.h"

class MotorController {
private:
	Motor motor;
public:
	MotorController(Motor motor);
	void stopMotor();
	void rotateMotor();
};

#endif