// file WaterSensor.cpp

#include "WaterSensor.h"

WaterSensor::WaterSensor(UART uart) : uart(uart) {
	speed = 0;
}

int WaterSensor::getWaterLevel() {
	uart.sendCommand(0x06, 0x00);

	// wachten op antwoord
	int level = 0; //

	return level;
}