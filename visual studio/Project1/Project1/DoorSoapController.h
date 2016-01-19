/**
 * \class The DoorSoapController controller class
 */

#pragma once
#ifndef _DOORSOAPCONTROLLER_H
#define _DOORSOAPCONTROLLER_H

#include "Door.h"
#include "SoapDispenser.h"
#include "WashingMachineController.h"
#include "pRTOS.h"
#include "UART.h"

class DoorSoapController : public RTOS::task {
private:
	Door door;
	SoapDispenser soap;
	UART uart;
	WashingMachineController wascontroller;
	RTOS::flag response_flag, new_job_flag;
	RTOS::pool< int > motor_job_pool, motor_time_pool;
	RTOS::pool< char * > response_pool;
	RTOS::mutex motor_job_mutex, motor_time_mutex, response_mutex;
	RTOS::timer rotate_timer;
	int taak;

	void stopMotor();

	//uart:
	char* readResponse();
	char* uartTask(char * command);

public:
	DoorSoapController(Door & door, SoapDispenser & soap, WashingMachineController & wascontroller);

	//uart:
	void setResponseFlag();
	void writeResponse(char * response);

	void main();
};

#endif