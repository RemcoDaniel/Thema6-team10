// file WaterController.h

#ifndef _WATERCONTROLLER_H
#define _WATERCONTROLLER_H

#include "pRTOS.h"
#include "WaterSensor.h"
#include "WashingMachineController.h"
#include "Pump.h"
#include "Valve.h"

class WaterController : public RTOS::task {
private:
	WaterSensor watersensor;
	WashingMachineController wascontroller;
	Pump pump;
	Valve valve;
	RTOS::pool< int > water_level_pool;
	RTOS::mutex water_level_mutex;
	RTOS::clock interval_clock;

	int getWaterLevel();

public:
	WaterController(WaterSensor watersensor, Pump pump, Valve valve, WashingMachineController wascontroller);
	void setWaterLevel(int level);

	void main();
};

#endif