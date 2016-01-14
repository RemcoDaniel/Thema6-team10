// file UART.h

#ifndef _UART_H
#define _UART_H

#include <string>

class UART{
public:
	UART();
	void sendCommand(unsigned int byte1, unsigned int byte2);
private:

};

#endif