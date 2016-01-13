// file Pump.cpp

#include "Pump.h"

Pump::Pump(UART uart) : uart(uart) {}

void Pump::on() {
	uart.sendCommand(0x05, 0x10);
}

void Pump::off() {
	uart.sendCommand(0x05, 0x20);
}

void Pump::getStatus() {
	uart.sendCommand(0x05, 0x01);
}