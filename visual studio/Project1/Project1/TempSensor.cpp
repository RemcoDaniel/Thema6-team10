// file TempSensor.cpp

#include "TempSensor.h"

TempSensor::TempSensor(UART uart) : uart(uart) {}

int TempSensor::getTemp() {
	uart.sendCommand(0x08, 0x00);


	int temp = 0;

	// wachten op antwoord....
	return temp;
}