// file WaterSensor.cpp

#include "WaterSensor.h"

WaterSensor::WaterSensor(UART uart) : uart(uart) {
	speed = 0;
}

void WaterSensor::getWaterLevel() {
	uart.sendCommand(0x06, 0x00);
}