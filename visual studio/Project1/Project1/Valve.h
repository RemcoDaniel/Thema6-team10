// file Valve.h

#ifndef _VALVE_H
#define _VALVE_H

#include "WaterController.h"
#include "UART.h"

class Valve {
private:
	int speed;
	UART &uart;
public:
	Valve(UART uart);
	void on();
	void off();
	void getStatus();
};

#endif