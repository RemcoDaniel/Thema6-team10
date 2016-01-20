// file Heater.h
#pragma once
#ifndef _HEATER_H
#define _HEATER_H

#include "Protocol.h"
#include <stdint.h>

class Heater {
public:
	Heater();
	char * getOnCommand();
	char * getOffCommand();
	char * getStatusCommand();
};

#endif