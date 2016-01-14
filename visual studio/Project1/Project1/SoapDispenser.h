// file SoapDispenser.h

#ifndef _SOAPDISPENSER_H
#define _SOAPDISPENSER_H

//#include "WaterController.h"
#include "UART.h"

class SoapDispenser {
private:
	UART &uart;
public:
	SoapDispenser(UART uart);
	void open();
	void close();
	void getStatus();
};

#endif