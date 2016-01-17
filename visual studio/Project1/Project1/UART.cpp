// file UART.cpp

#include "UART.h"
#include <iostream>

class uart_error : public std::exception {
public:
	uart_error(const std::string &error) : s(std::string{ "Something went wrong trying to open the uart port on port: [" } + error + "]")
	{}
	const char * what() const override {
		return s.c_str();
	}
private:
	std::string s;
};

UART::UART(const char * device, unsigned int baudrate, MotorController motorctrl, TempController tempctrl, WaterController waterctrl, WashingMachineController wasctrl):
	task{ 0, "uart" },			// de priority moet nog goed worden ingesteld
	device{ device },
	baudrate{baudrate},
	motorctrl(motorctrl),
	tempctrl(tempctrl),
	waterctrl(waterctrl),
	wasctrl(wasctrl),
	commandchannel(this, "channel")
{
	int portMakeState = theSerialPort.open(device, baudrate);
	if (portMakeState < 1){
		throw(uart_error(std::string{ "" } + device + " with error value " + char(portMakeState)));
	}
}

void UART::executeCommand(char * s){
	theSerialPort.writeString(s);

	while(theSerialPort.peek() < 2){
		// hier moet wat in komen te staan, anders kan de compiler dit misschien overslaan!
	}

	char * response;
	theSerialPort.readString(response, char(0xFF), 2);
	returnResponse(response);
}

char * UART::readChannel() {
	return commandchannel.read();
}

void UART::returnResponse(char * response) {
	switch (response[0]) {
		case '0x09':
		case '0x0A': 	motorctrl.writeResponse(response); motorctrl.setResponseFlag(); break;	// motortaak
		case '0x07':
		case '0x08': 	tempctrl.writeResponse(response); tempctrl.setResponseFlag();	break;	// temperatuurtaak
		case '0x03':
		case '0x05':
		case '0x06': 	waterctrl.writeResponse(response); waterctrl.setResponseFlag();	break;	// watertaak
		case '0x01':
		case '0x02':
		case '0x04':
		case '0x0B': 	wasctrl.writeResponse(response); wasctrl.setResponseFlag();		break;	// wasmachinetaak
	}
}

void UART::writeChannel(char * request) {
	commandchannel.write(request);
}

void UART::main() {
	for (;;) {
		// hier moet hij dingen gaan doen

		char * command = readChannel();
	}
}