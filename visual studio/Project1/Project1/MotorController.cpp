// file MotorController.cpp

#include "MotorController.h"

MotorController::MotorController(Motor motor, WashingMachineController wascontroller) : 
	motor(motor),
	wascontroller(wascontroller),
	task{ 0, "motorctrl" },		// priority, name
	new_motor_job_flag(this, "new_motor_job"),
	motor_job_pool("motor_job"),
	motor_job_mutex("motor_job"),
	motor_time_pool("motor_time"),
	motor_time_mutex("motor_time"),
	rotate_timer(this, "rotate_timer")
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

void MotorController::setMotorJob(int job, int time) {
	// eerst de job erin zetten:
	motor_job_mutex.wait();
	motor_job_pool.write(job);
	motor_job_mutex.signal();

	// nu nog de tijd zetten:
	motor_time_mutex.wait();
	motor_time_pool.write(time);
	motor_time_mutex.signal();
}

void MotorController::normalMotorJob(int time) {
	int speed = 500;
	for (int i = 0; i < 4; i++) {
		if (i == 2) {
			speed = 1000;
		}
		rotateLeft(speed);
		rotate_timer.set((time/2)/4);
		wait(rotate_timer);

		rotateRight(speed);
		rotate_timer.set((time/2)/4);
		wait(rotate_timer);
	}
	stopMotor();
}

void MotorController::centrifuge(int time) {
	int speed = 100;
	for (int i = 1; i < 16; i++) {
		rotateLeft(speed * i);
		rotate_timer.set((time / 2) / 16);
		wait(rotate_timer);
	}
	speed = 100;
	for (int i = 1; i < 16; i++) {
		rotateRight(speed * i);
		rotate_timer.set((time / 2) / 16);
		wait(rotate_timer);
	}
}

void MotorController::startMotorJob() {
	// job lezen
	motor_job_mutex.wait();
	int job = motor_job_pool.read();
	motor_job_mutex.signal();

	// tijd lezen
	motor_time_mutex.wait();
	int time = motor_time_pool.read();
	motor_time_mutex.signal();

	switch (job) {
		case 1:	normalMotorJob(time); break;
		case 2: centrifuge(time); break;
	}
}

int MotorController::getMotorSpeed() {
	int speed = motor.getMotorSpeed();
	return speed;
}

void MotorController::main() {
	for (;;) {
		wait(new_motor_job_flag);		// wachten tot een nieuwe job aanwezig is
		startMotorJob();				// deze job uit gaan voeren
		wascontroller.setMotorDone();	//	flag zetten
	}
}

