// file MotorController.cpp

#include "MotorController.h"

MotorController::MotorController(Motor motor) : motor{motor} {}

void MotorController::stopMotor() {
	motor.turn(0, 0);
}

void MotorController::rotateMotor(int speed) {		// dit moet het programma zijn dat hij moet draaien!
	motor.turn(0, 0);
	wait();
	motor.turn(0, 0);
	wait();
}