// file Heater.h

#ifndef _HEATER_H
#define _HEATER_H

#include "UART.h"

class Heater {
private:
	UART &uart;
public:
	Heater(UART uart);
	void on();
	void off();
	void getStatus();
};

#endif