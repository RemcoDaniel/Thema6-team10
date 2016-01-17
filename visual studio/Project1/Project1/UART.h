// file UART.h

#ifndef _UART_H
#define _UART_H

#include <string>
#include "libserial.h"
#include "pRTOS.h"
#include "MotorController.h"
#include "TempController.h"
#include "WaterController.h"
#include "WashingMachineController.h"

class UART : public RTOS::task {
private:
	MotorController motorctrl;
	TempController tempctrl;
	WaterController waterctrl;
	WashingMachineController wasctrl;
	const char * device;
	unsigned int baudrate;
	LibSerial theSerialPort;
	RTOS::channel< char *, 100 > commandchannel;

	char * readChannel();
	void returnResponse(char * response);

public:
	UART(const char * device, unsigned int baudrate, MotorController motorctrl, TempController tempctrl, WaterController waterctrl, WashingMachineController wasctrl);
	void executeCommand(char * s);

	void writeChannel(char * request);

	void main();
};

#endif