// file MotorController.h

#ifndef _MOTORCONTROLLER_H
#define _MOTORCONTROLLER_H

#include "Motor.h"
#include "WashingMachineController.h"
#include "pRTOS.h"
#include "UART.h"
#include <stdlib.h>
#include <memory>

using namespace std;

class MotorController : public RTOS::task {
private:
	Motor motor;
	shared_ptr<UART> uartptr;

	WashingMachineController wascontroller;
	RTOS::flag response_flag, new_job_flag;
	RTOS::pool< int > motor_job_pool, motor_time_pool;
	RTOS::pool< char * > response_pool;
	RTOS::mutex motor_job_mutex, motor_time_mutex, response_mutex;
	RTOS::timer rotate_timer;
	int taak;

	void stopMotor();
	void rotateRight(int speed);
	void rotateLeft(int speed);

	void normalMotorJob(int time);
	void centrifuge(int time);
	void startMotorJob();

	//uart:
	char* readResponse();
	char* uartTask(char * command);

public:
	MotorController(Motor & motor, WashingMachineController & wascontroller, shared_ptr<UART> uartptr);
	void setUartPointer(shared_ptr<UART> uartptr);

	int getMotorSpeed();		// getal van 0 - 1600 rpm
	void setMotorJob(int job, int time);

	//uart:
	void setResponseFlag();
	void writeResponse(char * response);

	void main();
};

#endif