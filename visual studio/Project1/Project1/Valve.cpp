// file Valve.cpp

#include "Valve.h"

Valve::Valve(UART uart) : uart(uart) {}

void Valve::on() {
	uart.sendCommand(0x03, 0x10);
}

void Valve::off() {
	uart.sendCommand(0x03, 0x20);
}

void Valve::getStatus() {
	uart.sendCommand(0x03, 0x01);
}