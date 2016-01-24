// file UART.cpp

#include "UART.h"
#include <iostream>

UART::UART(const char * device, unsigned int baudrate, MotorController * motorctrl, TempController * tempctrl, WaterController * waterctrl, WashingMachineController * wasctrl) :
	task{ 0, "uart" },			// de priority moet nog goed worden ingesteld
	device{ device },
	baudrate{ baudrate },
	motorctrl{ motorctrl },
	tempctrl{ tempctrl },
	waterctrl{ waterctrl },
	wasctrl{ wasctrl },
	interval_clock{ this, 50 MS, "interval" },
	commandchannel{ this, "channel" }
{
	int portMakeState = theSerialPort.open(device, baudrate);
	//if (portMakeState < 1){
	//	throw(uart_error(std::string{ "" } + device + " with error value " + char(portMakeState)));
	//}
}

void UART::executeCommand(char request, char command) {
	theSerialPort.flush();
	char s[3]{ request, command, '\0' };
	theSerialPort.writeString(s);

	while (theSerialPort.peek() < 2) {
		wait(interval_clock);
	}

	char response[3];
	theSerialPort.read(&response, 2);
	theSerialPort.flush();
	returnResponse(response[0], response[1]);
}

void UART::readChannel() {
	char response = commandchannel.read();
	char command = commandchannel.read();
	executeCommand(response, command);
}

void UART::returnResponse(char response, char command) {
	switch (response) {
		case 0x89:
		case 0x8A: 	motorctrl->writeResponse(command); motorctrl->setResponseFlag();	break;	// motortaak
		case 0x87:
		case 0x88: 	tempctrl->writeResponse(command); tempctrl->setResponseFlag();		break;	// temperatuurtaak
		case 0x83:
		case 0x85:
		case 0x86: 	waterctrl->writeResponse(command); waterctrl->setResponseFlag();	break;	// watertaak
		case 0x81:
		case 0x82:
		case 0x84:
		case 0x8B: 	wasctrl->writeResponse(command); wasctrl->setResponseFlag();		break;	// wasmachinetaak
		default:		std::cout << "foutafhandeling moet hier!\n";					break;
	}
}

void UART::writeChannel(char request, char command) {
	commandchannel.write(request);
	commandchannel.write(command);
}

void UART::main() {
	//=========================================================================
	// dit moet eerst!!!:
	executeCommand(MACHINE_REQ, START_CMD);
	// nu moet de washingmachinecontroller eerst wachten op de eerste flag! (zie washingmachinecontroller)
	//=========================================================================
	std::cout << "uart started\n";
	for (;;) {
		readChannel();
		wait(interval_clock);
	}
}