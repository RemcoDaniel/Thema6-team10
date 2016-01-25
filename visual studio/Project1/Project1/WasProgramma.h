// file Wasprogramma.h

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

	/*!	\fn int getLevel()
	*	\brief returns the waterlevel
	*/
	int getLevel();
	/*!	\fn int getLevel()
	*	\brief returns the temprature
	*/
	int getTemp();
	/*!	\fn int getTemp()
	*	\brief returns the time
	*/
	unsigned long int getTime();
	/*!	\fn unsigned long int getTime()
	*	\brief returns the time
	*/
	int getJob();
};

#endif
