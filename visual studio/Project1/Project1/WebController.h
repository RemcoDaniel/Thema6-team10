// file WebController.h
#pragma once
#ifndef _WEBCONTROLLER_H
#define _WEBCONTROLLER_H

#include "WashingMachineController.h"
#include "pRTOS.h"
#include <stdlib.h>
#include <memory>

class WashingMachineController;

class WebController : public RTOS::task {
private:
	RTOS::pool< int > temp_pool, water_pool, motor_pool;
	RTOS::mutex temp_mutex, water_mutex, motor_mutex;
	RTOS::clock interval_clock;

	int temp, water, motor;

	void getTemp();
	void getWaterLevel();
	void getMotorRPM();

	void logging();

public:
	WebController();

	void setTemp(int temp);
	void setWaterLevel(int water);
	void setMotorRPM(int motor);

	void main();
};

#endif