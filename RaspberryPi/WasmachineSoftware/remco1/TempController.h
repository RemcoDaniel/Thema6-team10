// file TempController.h
#pragma once
#ifndef _TEMPCONTROLLER_H
#define _TEMPCONTROLLER_H

#include "Heater.h"
#include "TempSensor.h"
#include "WashingMachineController.h"
#include "pRTOS.h"
#include "UART.h"

class WashingMachineController;
class UART;

class TempController : public RTOS::task {
private:
	char * command = 0;
	Heater heater;
	TempSensor tempsensor;
	UART *uartptr;
	WashingMachineController *wascontroller;
	RTOS::flag response_flag, start_flag;
	RTOS::pool< int > temp_pool;
	RTOS::pool< char > response_pool;
	RTOS::mutex temp_mutex, response_mutex;
	RTOS::clock interval_clock;

	int getNewTemp();
	int getTemp();
	void heat(bool on);		// on = 1 , off = 0

	//uart:
	char readResponse();
	char uartTask(char request, char command);

public:
	TempController(WashingMachineController * wascontroller);
	void setUartPointer(UART * u);

	void setTemp(int temp);
	void startTempController();

	//uart:
	void setResponseFlag();
	void writeResponse(char response);

	void main();
};

#endif