// file WashingMachineController.h

#ifndef _WASHINGMACHINECONTROLLER_H
#define _WASHINGMACHINECONTROLLER_H

//#include .....

class WashingMachineController {
private:
	//time_t time;
	//currentWasprog wasprogramma;
	//TemperaturController theTempController;
	//WaterController theWaterController;
	//MotorController theMotorController;

public:
	WashingMachineController();
	
	void setUART();
	void registerObserver();
	void removeObserver();
	void notifyObserver();
	void washingMachineController(); //???????????
	//void startWasprogramma(Wasprogramma wp);
	void stopWasprogramma();
	void start();
	void stop();
	void errorHandling();
};

#endif