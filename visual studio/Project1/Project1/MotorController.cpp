<<<<<<< HEAD
#include "MotorController.h"
=======
// file MotorController.cpp

#include "MotorController.h"

MotorController::MotorController(Motor motor) : 
	motor{motor},
	task{ 0, "motorctrl" }		// priority, name
	interval_clock{ this, 500 * bmptk::us, "interval" },
	new_motor_job_flag(this, "new_motor_job"),
	motor_job_pool("motor_job"),
	motor_job_mutex("motor_job")
{}


void MotorController::stopMotor() {
	motor.turn(0, 0);
}

void MotorController::rotateMotor(int speed) {		// dit moet het programma zijn dat hij moet draaien!
	motor.turn(0, 0);
	wait();
	motor.turn(0, 0);
	wait();
}
<<<<<<< HEAD

// deze moet nog iets van een struct hebben waarin de programmas staan

void MotorController::main() {
	for (;;) {
		wait(interval_clock);
		int newtemp = getTemp();
		int temp = tempsensor.getTemp();
		if (temp >= newtemp) {
			heater.off();
		}
		else heater.on();
	}
}
=======
>>>>>>> a7edb3850a3833b06e387c5487ffe5a2285ff29f
>>>>>>> 02df9de188cf52ea9fca72b64622f49551a41f45
