// file UART.h

#ifndef _UART_H
#define _UART_H

#include <string>
#include "libserial.h"

class UART{
public:
	UART(const char * device, unsigned int baudrate);
	void sendCommand(unsigned int byte1, unsigned int byte2);
	int readAnswer();
private:
	const char * device;
	unsigned int baudrate;
	LibSerial theSerialPort;
};

#endif