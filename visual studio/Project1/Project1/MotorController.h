// file MotorController.h

#ifndef _MOTORCONTROLLER_H
#define _MOTORCONTROLLER_H

#include "Motor.h"
#include "pRTOS.h"

class MotorController : public RTOS::task {
private:
	Motor motor;
	RTOS::flag new_motor_job_flag;
	RTOS::pool< int > motor_job_pool;
	RTOS::mutex motor_job_mutex;
	RTOS::clock interval_clock;

	void stopMotor();
	void rotateMotor();

public:
	MotorController(Motor motor);
	void normalMotorJob();
	void centrifuge();

	void main();
};

#endif