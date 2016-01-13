// file Pump.h

#ifndef _PUMP_H
#define _PUMP_H

#include "WaterController.h"
#include "UART.h"

class Pump {
private:
	int speed;
	UART &uart;
public:
	Pump(UART uart);
	void on();
	void off();
	void getStatus();
};

#endif