// file MotorController.h
#pragma once
#ifndef _MOTORCONTROLLER_H
#define _MOTORCONTROLLER_H

#include "Motor.h"
#include "WashingMachineController.h"
#include "pRTOS.h"
#include "UART.h"
#include <stdlib.h>
#include <memory>

class WashingMachineController;
class UART;

class MotorController : public RTOS::task {
private:
	char * command = 0;
	Motor motor;
	UART *uartptr;
	WashingMachineController * wascontroller;
	RTOS::flag response_flag, new_job_flag, start_flag;
	RTOS::pool< int > motor_job_pool, motor_time_pool;
	RTOS::pool< char > response_pool;
	RTOS::mutex motor_job_mutex, motor_time_mutex, response_mutex;
	RTOS::timer rotate_timer;
	int job, time, speed;

	void stopMotor();
	void rotateRight(int speed);
	void rotateLeft(int speed);

	void normalMotorJob(unsigned long int time);
	void centrifuge(unsigned long int time);
	void startMotorJob();

	//uart:
	char readResponse();
	char uartTask(char request, char command);

public:
	MotorController(WashingMachineController * wascontroller);
	void setUartPointer(UART *u);

	void startMotorController();

	int getMotorSpeed();		// getal van 0 - 1600 rpm
	void setMotorJobTime(unsigned long int time);

	//uart:
	void setResponseFlag();
	void writeResponse(char response);

	void main();
};

#endif