// file Door.h

#ifndef _DOOR_H
#define _DOOR_H

#include "Protocol.h"
#include <stdint.h>

class Door {
private:
	char lockcommand[3] = { DOOR_LOCK_REQ, LOCK_CMD, '\0' };
	char unlockcommand[3] = { DOOR_LOCK_REQ, UNLOCK_CMD, '\0' };
	char statuscommand[3] = { DOOR_LOCK_REQ, STATUS_CMD, '\0' };
	
public:
	Door();
	char * getLockCommand();
	char * getUnlockCommand();
	char * getStatusCommand();
};

#endif