// file Heater.cpp

#include "Heater.h"

Heater::Heater(UART uart) : uart(uart) {}

void Heater::on() {
	uart.sendCommand(0x07, 0x10);
}

void Heater::off() {
	uart.sendCommand(0x07, 0x20);
}

void Heater::getStatus() {
	uart.sendCommand(0x07, 0x01);
}