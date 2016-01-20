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
	char * getLockCommand();
	/*! \fn char * getLockCommand()
	 *	\brief Returns the bytes to set the lock on
	 *	\return a char pointer to a 2 char array
	 */
	char * getUnlockCommand();
	/*! \fn char * getUnlockCommand()
	 *	\brief Returns the bytes to set the lock off
	 *	\return a char pointer to a 2 char array
	 */
	char * getStatusCommand();
	/*! \fn char * getStatusCommand()
	 *	\brief Returns the bytes to get the status of the lock
	 *	\return a char pointer to a 2 char array
	 */
};

#endif