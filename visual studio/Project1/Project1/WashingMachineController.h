// file WashingMachineController.h

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

class WashingMachineController : public RTOS::task {
private:
	Wasprogramma wasprogramma;
	Door door;
	TempController tempcontroller;
	WaterController watercontroller;
	MotorController motorcontroller;
	UART uart;
	SoapDispenser soap;
	RTOS::flag temp_reached_flag, level_reached_flag, motor_done_flag, response_flag;
	RTOS::pool< char * > response_pool;
	RTOS::mutex response_mutex;
	RTOS::clock interval_clock;

	void doorlock(bool lock);		// lock = 1 , unlock = 0
	void dispendSoap();

	//uart:
	char* readResponse();
	char* uartTask(char * command);

public:
	WashingMachineController(Door door, TempController temp, WaterController water, MotorController motor, Wasprogramma was, UART uart, SoapDispenser soap);

	void setTempReached();
	void setWaterLevelReached();
	void setMotorDone();

	void startWasprogramma();
	void stopWasprogramma();

	//uart:
	void setResponseFlag();
	void writeResponse(char * response);

	void main();
};

#endif