// file TempSensor.h

#ifndef _TEMPSENSOR_H
#define _TEMPSENSOR_H

//#include "TempController.h"
#include "UART.h"

class TempSensor {
private:
	UART &uart;
public:
	TempSensor(UART uart);
	void on();
	int getTemp();
};

#endif