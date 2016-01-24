// file WasProgramma.cpp

#include "WasProgramma.h"

Wasprogramma::Wasprogramma(int temp, int waterlevel, int time, int job) :
	temp(temp),
	waterlevel(waterlevel),
	time(time),
	job(job)
{}

int Wasprogramma::getTemp() {
	return temp;
}

int Wasprogramma::getLevel() {
	return waterlevel;
}

int Wasprogramma::getTime() {
	return time;
}

int Wasprogramma::getJob() {
	return job;
}

