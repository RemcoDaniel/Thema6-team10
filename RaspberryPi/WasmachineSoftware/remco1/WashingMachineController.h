// file WashingMachineController.h
#pragma once
#ifndef _WASHINGMACHINECONTROLLER_H
#define _WASHINGMACHINECONTROLLER_H

#include "pRTOS.h"
#include "MotorController.h"
#include "WaterController.h"
#include "TempController.h"
#include "Door.h"
#include "Wasprogramma.h"
#include "UART.h"
#include "SoapDispenser.h"

class UART;
class MotorController;
class WaterController;
class TempController;

class WashingMachineController : public RTOS::task {
private:
	char * command = 0;
	Wasprogramma wasprogramma;
	Door door;
	TempController *tempcontroller;
	WaterController *watercontroller;
	MotorController *motorcontroller;
	UART *uart;
	SoapDispenser soap;
	RTOS::flag temp_reached_flag, level_reached_flag, motor_done_flag, response_flag;
	RTOS::pool< char > response_pool;
	RTOS::mutex response_mutex;
	RTOS::clock interval_clock;

	void doorlock(bool lock);		// lock = 1 , unlock = 0
	bool getDoorStatus();			// open = 1, locked or closed = 0;
	void dispendSoap();
	void signalLed(bool on);

	//uart:
	char readResponse();
	char uartTask(char request, char command);

public:
	WashingMachineController(Wasprogramma & was);

	void setTempReached();
	void setWaterLevelReached();
	void setMotorDone();

	void startWasprogramma();
	void stopWasprogramma();

	//uart:
	void setResponseFlag();
	void writeResponse(char response);

	void main();
};

#endif