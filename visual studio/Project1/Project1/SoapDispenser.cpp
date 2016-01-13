// file SoapDispenser.cpp

#include "SoapDispenser.h"

SoapDispenser::SoapDispenser(UART uart) : uart(uart) {}

void SoapDispenser::open() {
	uart.sendCommand(0x04, 0x10);
}

void SoapDispenser::close() {
	uart.sendCommand(0x04, 0x20);
}

void SoapDispenser::getStatus() {
	uart.sendCommand(0x04, 0x01);
}