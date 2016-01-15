// file UART.cpp

#include "UART.h"
#include <iostream>

//class uart_error : public std::exception {
//public:
//	uart_error(const std::string &error) : s(std::string{ "Something went wrong trying to open the uart port on port: [" } + error + "]")
//	{}
//	const char * what() const override {
//		return s.c_str();
//	}
//private:
//	std::string s;
//};

UART::UART(const char * device, unsigned int baudrate):
device{ device },
baudrate{baudrate}
{
	int portMakeState = theSerialPort.open(device, baudrate);
	if (portMakeState < 1){
		std::cout << "Error opening port"
		//throw(uart_error(std::string{ "" } + device + " with error value " + char(portMakeState)));
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
	
}

char * UART::executeCommand(char Char1, char Char2){
	//theSerialPort.writeString(s);
	theSerialPort.writeChar(Char1);
	theSerialPort.writeChar(Char2);
	while(theSerialPort.peek() < 2){
	}

	char * response = 0;
	theSerialPort.readString(response, char(0xFF), 2);

	return response;
}