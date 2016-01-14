// file TempSensor.h

#ifndef _TEMPSENSOR_H
#define _TEMPSENSOR_H

#include "UART.h"

class TempSensor {
private:
	UART &uart;
public:
	TempSensor(UART uart);
	int getTemp();
};

#endif