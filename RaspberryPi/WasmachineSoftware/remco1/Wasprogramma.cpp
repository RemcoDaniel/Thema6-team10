// file WasProgramma.cpp

#include "Wasprogramma.h"

Wasprogramma::Wasprogramma(int temp, int waterlevel, unsigned long int time, int job) :
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

unsigned long int Wasprogramma::getTime() {
	return time;
}

int Wasprogramma::getJob() {
	return job;
}