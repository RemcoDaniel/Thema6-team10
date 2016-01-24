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
#include "Protocol.h"
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
	RTOS::pool< int > temp_pool, water_pool, motor_pool, status_pool;
	RTOS::mutex temp_mutex, water_mutex, motor_mutex, status_mutex;
	RTOS::clock interval_clock;

	int temp, water, motor, status;

	void getTemp();
	void getWaterLevel();
	void getMotorRPM();
	void getStatus();

	void logging();
	void sendStatus();
	void messageHandling();

public:
	WebController(WasmachineApp * app, WashingMachineController * was, MotorController * motor, TempController * temp, WaterController * water);

	void setTemp(int temp);
	void setWaterLevel(int water);
	void setMotorRPM(int motor);
	void setStatus(int status);

	void main();
};

#endif