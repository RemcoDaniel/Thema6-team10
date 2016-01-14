// file TempController.h

#ifndef _TEMPCONTROLLER_H
#define _TEMPCONTROLLER_H

#include "Heater.h"
#include "TempSensor.h"
#include "pRTOS.h"

class TempController : public RTOS::task {
private:
	Heater heater;
	TempSensor tempsensor;
	RTOS::pool< int > temp_pool;
	RTOS::mutex temp_mutex;
	RTOS::clock interval_clock;

	int getTemp();

public:
	TempController(Heater heater, TempSensor tempsensor);
	void setTemp(int temp);

	void main();
};

#endif