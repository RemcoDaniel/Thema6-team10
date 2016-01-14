// file WashingMachineController.h

#ifndef _WASHINGMACHINECONTROLLER_H
#define _WASHINGMACHINECONTROLLER_H

#include "pRTOS.h"
#include "MotorController.h"
#include "WaterController.h"
#include "TempController.h"

class WashingMachineController : public RTOS::task {
private:
	//time_t time;
	//currentWasprog wasprogramma;
	TempController tempcontroller;
	WaterController watercontroller;
	MotorController motorcontroller;
	RTOS::flag temp_reached_flag, level_reached_flag, motor_done_flag;
	RTOS::clock interval_clock;

public:
	WashingMachineController(TempController tempcontroller, WaterController watercontroller, MotorController motorcontroller);

	void setTempReached();
	void setWaterLevelReached();
	void setMotorDone();

	void registerObserver();
	void removeObserver();
	void notifyObserver();

	void loadWasprogramma(int temp, int water, int time);

	void startWasprogramma(Wasprogramma wp);
	void stopWasprogramma();

	void main();
};

#endif