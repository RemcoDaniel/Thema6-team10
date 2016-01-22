// file UART.cpp

#include "UART.h"
#include <iostream>

UART::UART(const char * device, unsigned int baudrate):
device{ device },
baudrate{baudrate}
{
	int portMakeState = theSerialPort.open(device, baudrate);
	if (portMakeState < 1){
		std::cout << "Error opening port" + char(portMakeState);
	}
}

int UART::readAnswer(){
	return 0;
}

char * UART::executeCommand(const char * s){
	std::cout << s << "\n";
	theSerialPort.writeString(s);

	while(theSerialPort.peek() < 2){
	}

	char * response = 0;
	theSerialPort.readString(response, char(0xFF), 2);
	
	std::cout << response << " msg\n";
	
	return response;
}
