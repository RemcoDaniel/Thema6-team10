// file WashingMachineController.h

#ifndef _WASHINGMACHINECONTROLLER_H
#define _WASHINGMACHINECONTROLLER_H

//#include "TempController.h"
#include "pRTOS.h"

class WashingMachineController : public RTOS::task {
private:
	//time_t time;
	//currentWasprog wasprogramma;
	//TemperaturController theTempController;
	//WaterController theWaterController;
	//MotorController theMotorController;
	RTOS::flag temp_reached_flag, level_reached_flag, motor_done_flag;
	RTOS::clock interval_clock;

public:
	WashingMachineController();

	void setTempReached();

	void registerObserver();
	void removeObserver();
	void notifyObserver();
	void washingMachineController(); //???????????
	//void startWasprogramma(Wasprogramma wp);
	void stopWasprogramma();
	void start();
	void stop();
	void errorHandling();

	void main();
};

#endif