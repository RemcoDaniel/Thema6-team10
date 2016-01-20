// file Wasprogramma.h

#ifndef _WASPROGRAMMA_H
#define _WASPROGRAMMA_H

class Wasprogramma {
private:
	int temp, waterlevel, job;
	unsigned long int time;
public:
	Wasprogramma(int temp, int waterlevel, unsigned long int time, int job);

	int getLevel();
	int getTemp();
	unsigned long int getTime();
	int getJob();
};

#endif