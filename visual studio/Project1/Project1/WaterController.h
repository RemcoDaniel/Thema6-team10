/**
 * \class The WaterController controller class
 */

#ifndef _WATERCONTROLLER_H
#define _WATERCONTROLLER_H

#include "pRTOS.h"
#include "WaterSensor.h"
#include "WashingMachineController.h"
#include "Pump.h"
#include "Valve.h"
#include "UART.h"

class WaterController : public RTOS::task {
private:
	WaterSensor watersensor;
	WashingMachineController wascontroller;
	Pump pump;
	Valve valve;
	shared_ptr<UART> uartptr;
	RTOS::flag response_flag;
	RTOS::pool< int > water_level_pool;
	RTOS::pool< char * > response_pool;
	RTOS::mutex water_level_mutex, response_mutex;
	RTOS::clock interval_clock;

	int getNewWaterLevel();
	int getWaterLevel();
	void pumping(bool on);		// on = 1 , off = 0
	void valving(bool on);		// on = 1 , off = 0

	//uart:
	char* readResponse();
	char* uartTask(char * command);

public:
	WaterController(WaterSensor & watersensor, Pump & pump, Valve & valve, WashingMachineController & wascontroller, shared_ptr<UART> uartptr);
	void setUartPointer(shared_ptr<UART> u);
	void setWaterLevel(int level);

	//uart:
	void setResponseFlag();
	void writeResponse(char * response);

	void main();
};

#endif