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
	
	/*!	\fn void executeCommand(char request, char command)
	 *	\brief Sends the given command and returns the response from the emulator
	 *	\param s the request you want to send
	 *	\return The response of the emulator
	 */
	void executeCommand(char request, char command);
	
	/*! \fn void writeChannel(char request, char command)
	 *	\brief Puts two chars on the UART
	 *	\param s The two chars you want to send over the UART
	 */
	void writeChannel(char request, char command));

	void main();
};

#endif