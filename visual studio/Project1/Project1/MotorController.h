// file MotorController.h

#ifndef _MOTORCONTROLLER_H
#define _MOTORCONTROLLER_H

#include "Motor.h"
#include "WashingMachineController.h"
#include "pRTOS.h"

class MotorController : public RTOS::task {
private:
	Motor motor;
	WashingMachineController wascontroller;
	RTOS::flag new_motor_job_flag;
	RTOS::pool< int > motor_job_pool, motor_time_pool;
	RTOS::mutex motor_job_mutex, motor_time_mutex;
	RTOS::timer rotate_timer;

	void stopMotor();
	void rotateRight(int speed);
	void rotateLeft(int speed);

	void normalMotorJob(int time);
	void centrifuge(int time);
	void startMotorJob();

public:
	MotorController(Motor motor, WashingMachineController wascontroller);

	int getMotorSpeed();		// getal van 0 - 1600 rpm
	void setMotorJob(int job, int time);

	void main();
};

#endif