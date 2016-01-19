/**
 * \class The Wasprogramma user class
 */

#ifndef _WASPROGRAMMA_H
#define _WASPROGRAMMA_H

class Wasprogramma {
private:
	int temp, waterlevel, time, job;
public:
	Wasprogramma(int temp, int waterlevel, int time, int job);

	int getLevel();
	int getTemp();
	int getTime();
	int getJob();
};

#endif