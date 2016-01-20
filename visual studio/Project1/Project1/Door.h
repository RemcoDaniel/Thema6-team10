/**
 * \class Door boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _DOOR_H
#define _DOOR_H

#include "Protocol.h"

class Door {
public:
	Door();
	char * getLockCommand(); ///< Returns the bytes for locking the door.
	char * getUnlockCommand();  ///< returns the bytes for unlocking the door.
	char * getStatusCommand(); ///< Returns the bytes for getting the status of the door
};

#endif