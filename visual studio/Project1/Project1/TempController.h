// file TempController.h

#ifndef _TEMPCONTROLLER_H
#define _TEMPCONTROLLER_H

#include "Heater.h"
#include "TempSensor.h"
#include "WashingMachineController.h"
#include "pRTOS.h"
#include "UART.h"

class TempController : public RTOS::task {
private:
	Heater heater;
	TempSensor tempsensor;
	UART uart;
	WashingMachineController wascontroller;
	RTOS::flag response_flag;
	RTOS::pool< int > temp_pool;
	RTOS::pool< char * > response_pool;
	RTOS::mutex temp_mutex, response_mutex;
	RTOS::clock interval_clock;

	int getNewTemp();
	int getTemp();
	void heat(bool on);		// on = 1 , off = 0

	//uart:
	char* readResponse();
	char* uartTask(char * command);

public:
	TempController(Heater heater, TempSensor tempsensor, WashingMachineController wascontroller, UART uart);
	void setTemp(int temp);

	//uart:
	void setResponseFlag();
	void writeResponse(char * response);

	void main();
};

#endif