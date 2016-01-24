// file WebController.h
#pragma once
#ifndef _WEBCONTROLLER_H
#define _WEBCONTROLLER_H

#include "WashingMachineController.h"
#include "WasmachineApp.h"
#include "WaterController.h"
#include "TempController.h"
#include "MotorController.h"
#include "pRTOS.h"
#include <stdlib.h>
#include <memory>

class WashingMachineController;
class WasmachineApp;
class TempController;
class WaterController;
class MotorController;

class WebController : public RTOS::task {
private:
	WasmachineApp * app;
	WashingMachineController * wasctrl;
	MotorController * motorctrl; 
	TempController * tempctrl; 
	WaterController * waterctrl;
	RTOS::pool< int > temp_pool, water_pool, motor_pool;
	RTOS::mutex temp_mutex, water_mutex, motor_mutex;
	RTOS::clock interval_clock;

	int temp, water, motor;

	void getTemp();
	void getWaterLevel();
	void getMotorRPM();

	void logging();
	void messageHandling();

public:
	WebController(WasmachineApp * app, WashingMachineController * was, MotorController * motor, TempController * temp, WaterController * water);

	void setTemp(int temp);
	void setWaterLevel(int water);
	void setMotorRPM(int motor);

	void main();
};

#endif