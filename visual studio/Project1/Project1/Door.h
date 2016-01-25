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
	/*! \fn char * getLockCommand()
	*	\brief Returns the bytes to lock the door
	*	\return a char pointer to a 2 char array
	*/
	char * getLockCommand();
	/*! \fn char * getUnlockCommand()
	*	\brief Returns the bytes to unlock the door
	*	\return a char pointer to a 2 char array
	*/
	char * getUnlockCommand();
	/*! \fn char * getStatusCommand()
	*	\brief Returns the bytes to get the status of the door
	*	\return a char pointer to a 2 char array
	*/
	char * getStatusCommand();
};

#endif
