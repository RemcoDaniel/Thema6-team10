// file MotorController.cpp

#include "MotorController.h"

MotorController::MotorController(Motor motor, WashingMachineController wascontroller) : 
	motor{motor},
	wascontroller(wascontroller),
	task{ 0, "motorctrl" },		// priority, name
	new_motor_job_flag(this, "new_motor_job"),
	motor_job_pool("motor_job"),
	motor_job_mutex("motor_job")
{}

void MotorController::stopMotor() {
	motor.turn(0, 0);
}

void MotorController::rotateLeft(int speed) {
	motor.turn(1, speed);
}

void MotorController::rotateRight(int speed) {
	motor.turn(0, speed);
}

void MotorController::setMotorJob(int job) {
	motor_job_mutex.wait();
	motor_job_pool.write(job);
	motor_job_mutex.signal();
}

void MotorController::normalMotorJob() {		// dit is slechts een voorbeeld!!!
	// linksom met 1000 rpm
	rotateLeft(1000);
	// 10 minuten lang
	// rechtsom met 1000 rpm
	rotateRight(1000);
	// 10 minuten lang
	stopMotor();
}

void MotorController::centrifuge() {
	// hier ff een zinnig programma ingooien
	rotateLeft(1600);
}

void MotorController::startMotorJob() {
	motor_job_mutex.wait();
	int job = motor_job_pool.read();
	motor_job_mutex.signal();

	switch (job) {
		case 1:	normalMotorJob(); break;
		case 2: centrifuge(); break;
	}
}

void MotorController::main() {
	for (;;) {
		wait(new_motor_job_flag);		// wachten tot een nieuwe job aanwezig is
		startMotorJob();				// deze job uit gaan voeren
		wascontroller.setMotorDone();	//	flag zetten
	}
}

