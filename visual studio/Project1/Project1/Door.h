// file Door.h

#ifndef _DOOR_H
#define _DOOR_H

#include "Protocol.h"

class Door {
public:
	Door();
	char * getLockCommand();
	char * getUnlockCommand();
	char * getStatusCommand();
};

#endif