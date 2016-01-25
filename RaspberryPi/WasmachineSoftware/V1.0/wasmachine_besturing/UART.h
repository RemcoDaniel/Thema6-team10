// file UART.h
#pragma once

#ifndef _UART_H
#define _UART_H

#include <string>
#include "libserial.h"
#include "pRTOS.h"
#include "MotorController.h"
#include "TempController.h"
#include "WaterController.h"
#include "WashingMachineController.h"

class TempController;
class WaterController;
class MotorController;
class WashingMachineController;

class UART : public RTOS::task {
private:
	MotorController *motorctrl;
	TempController *tempctrl;
	WaterController *waterctrl;
	WashingMachineController *wasctrl;
	const char * device;
	unsigned int baudrate;
	LibSerial theSerialPort;
	RTOS::channel< char, 100 > commandchannel;
	RTOS::clock interval_clock;

	void readChannel();
	void returnResponse(char response, char command);

public:
	UART(const char * device, unsigned int baudrate, MotorController * motorctrl, TempController * tempctrl, WaterController * waterctrl, WashingMachineController * wasctrl);
	void executeCommand(char request, char command);

	void writeChannel(char request, char command);

	void main();
};

#endif