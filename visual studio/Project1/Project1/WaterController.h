// file WaterController.h

#ifndef _WATERCONTROLLER_H
#define _WATERCONTROLLER_H

#include "pRTOS.h"

class WaterController : public RTOS::task {
private:
	RTOS::flag new_level_flag;
	RTOS::pool< int > water_level_pool;
	RTOS::mutex water_level_mutex;
public:
	WaterController();

	float getWaterLevel();
	void setWaterLevel(float wlvl);
	bool getValveStatus();
	void setValveStatus(bool state);
	void updateState();

	void main();
};

#endif