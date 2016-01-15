// file Door.h

#ifndef _DOOR_H
#define _DOOR_H

#include "UART.h"

class Door {
private:
	UART uart;
public:
	Door(UART uart);
	void lock();
	void unlock();
	void getStatus();
};

#endif