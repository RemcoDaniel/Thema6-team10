/**
 * \class The UART boundry class
 */
 
#pragma once
#ifndef _UART_H
#define _UART_H

#include <string>
#include "libserial.h"
#include "pRTOS.h"
#include "MotorController.h"
class MotorController;
#include "TempController.h"
class TempController;
#include "WaterController.h"
class WaterController;
#include "WashingMachineController.h"
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
	RTOS::channel< char *, 100 > commandchannel;
	RTOS::clock interval_clock;

	void readChannel();
	void returnResponse(char * response);

public:
	UART(const char * device, unsigned int baudrate, MotorController * motorctrl, TempController * tempctrl, WaterController * waterctrl, WashingMachineController * wasctrl);
	void executeCommand(char * s);

	void writeChannel(char * request);

	void main();
};

#endif