// file Door.cpp

#include "Door.h"

Door::Door(UART uart) :
	uart(uart)
{}

void Door::lock() {
	uart.sendCommand(0x02, 0x40);
}

void Door::unlock() {
	uart.sendCommand(0x02, 0x80);
}

void Door::getStatus() {
	uart.sendCommand(0x02, 0x01);
}