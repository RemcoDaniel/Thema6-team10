// file WaterSensor.h

#ifndef _WATERSENSOR_H
#define _WATERSENSOR_H

#include "WaterController.h"
#include "UART.h"

class WaterSensor {
private:
	int speed;
	UART &uart;
public:
	WaterSensor(UART uart);
	void getWaterLevel();
};

#endif