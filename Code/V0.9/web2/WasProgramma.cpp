// file WasProgramma.cpp

#include "WasProgramma.h"

Wasprogramma::Wasprogramma(int t, int waterlevel, unsigned long int time, int job) :
	temp(t),
	waterlevel(waterlevel),
	time(time),
	job(job)
{
	std::cout << temp << " temp van wasprogramma\n";
	std::cout << waterlevel << " waterlevel van wasprogramma\n";
	std::cout << time << " time van wasprogramma\n";
	std::cout << job << " job van wasprogramma\n";
}

int Wasprogramma::getTemp() {
	std::cout << temp << " temp van wasprogramma\n";
	std::cout << waterlevel << " waterlevel van wasprogramma\n";
	std::cout << time << " time van wasprogramma\n";
	std::cout << job << " job van wasprogramma\n";
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

