// file TempSensor.cpp

#include "TempSensor.h"

TempSensor::TempSensor(UART uart) : uart(uart) {}

int TempSensor::getTemp() {
	return uart.sendCommand(TEMPERATURE_REQ, '\0');
}