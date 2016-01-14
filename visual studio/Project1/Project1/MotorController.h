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
	RTOS::pool< int > motor_job_pool;
	RTOS::mutex motor_job_mutex;

	void stopMotor();
	void rotateRight(int speed);
	void rotateLeft(int speed);

	void normalMotorJob();
	void centrifuge();

	void startMotorJob();

public:
	MotorController(Motor motor, WashingMachineController wascontroller);
	void setMotorJob(int job);
	void main();
};

#endif