/**
 * \class The Wasprogramma user class
 */

#ifndef _WASPROGRAMMA_H
#define _WASPROGRAMMA_H

#include <iostream>

struct wasprogrammaStruct{
	int temperature, waterlevel, time, job;

};

class Wasprogramma {
private:
	int temp, waterlevel, job;
	unsigned long int time;
public:
	Wasprogramma(int temp, int waterlevel, unsigned long int time, int job);

	/*!	\fn		int getLevel()
	 *	\return	Returns the newest requisted waterlevel
	 */
	int getLevel();
	
	/*!	\fn		int getTemp()
	 *	\return	Returns the newest requisted temp
	 */
	int getTemp();
	
	/*!	\fn		unsigned long int getTime()
	 *	\return	Returns the newest requisted time
	 */
	unsigned long int getTime();
	
	/*!	\fn		int getJob()
	 *	\return	Returns the newest requisted job
	 */
	int getJob();
};

#endif