// file Wasprogramma.h

#ifndef _WASPROGRAMMA_H
#define _WASPROGRAMMA_H

struct wasprogrammaStruct{
	int temperature, waterlevel, time, job;
};

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