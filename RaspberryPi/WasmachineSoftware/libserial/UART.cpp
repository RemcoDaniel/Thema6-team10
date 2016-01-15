// file UART.cpp

#include "UART.h"
#include <iostream>

UART::UART(const char * device, unsigned int baudrate):
device{ device },
baudrate{baudrate}
{
	int portMakeState = theSerialPort.open(device, baudrate);
	if (portMakeState < 1){
		std::cout <<  " with error value " + char(portMakeState);
	}
}

void UART::sendCommand(unsigned int byte1, unsigned int byte2) {
	printf("dit werkt\n");
	//printf("%u", code);

	unsigned int code = byte1 << 8 | byte2;
	printf("%x\n", byte1);
	printf("%x\n", byte2);
	printf("%x\n", code);

}

int UART::readAnswer(){
	return 0;
}

char * UART::executeCommand(const char * s){
	std::cout << s << "\n";
	theSerialPort.writeString(s);

	while(theSerialPort.peek() < 2){
		std::cout << "jup\n";
	}

	char * response = 0;
	theSerialPort.readString(response, char(0xFF), 2);
	
	return response;
}
