// file TempController.h

#ifndef _TEMPCONTROLLER_H
#define _TEMPCONTROLLER_H

#include "pRTOS.h"

class TempController : public RTOS::task {
private:
	RTOS::flag new_temp_flag;
	RTOS::pool< int > temp_pool;
	RTOS::mutex temp_mutex;
public:
	TempController();

	void main();
};

#endif