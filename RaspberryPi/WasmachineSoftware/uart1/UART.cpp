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

char * UART::executeCommand(char * s){
	theSerialPort.flush();
	std::cout << unsigned(s[0]) << "  " << unsigned(s[1]) << "\n";
	theSerialPort.writeString(s);

	while(theSerialPort.peek() < 2){
		std::cout << theSerialPort.peek() << " jup\n";
	}
	std::cout << theSerialPort.peek() << "eruit\n";
	
	char test[2];
	
	//while(theSerialPort.peek() > 0){
		theSerialPort.read(&test, 2);
		theSerialPort.flush();
		//if(test != ' '){
			std::cout << "Dit is test:" << unsigned(test[0]) << "   " << unsigned(test[1]) << ":einde"<< std::endl;
		//}
	//}
	//char * response = 0;
	//theSerialPort.readString(response, '\0', 1);
	
	//std::cout << response << " msg\n";
	char * t = 0;
	return t;
}
