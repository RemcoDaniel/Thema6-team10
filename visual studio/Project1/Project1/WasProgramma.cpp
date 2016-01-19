/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

#include "Wasprogramma.h"

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