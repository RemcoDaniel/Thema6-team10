// file UART.cpp

#include "UART.h"

UART::UART() {}

void UART::sendCommand(unsigned int byte1, unsigned int byte2) {
	printf("dit werkt\n");
	//printf("%u", code);

	unsigned int code = byte1 << 8 | byte2;
	printf("%x\n", byte1);
	printf("%x\n", byte2);
	printf("%x\n", code);
}