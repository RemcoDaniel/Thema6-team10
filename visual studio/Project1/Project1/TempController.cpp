#include "TempController.h"

TempController::TempController(Heater heater, TempSensor tempsensor, WashingMachineController wascontroller) :
	heater(heater),
	tempsensor(tempsensor),
	wascontroller(wascontroller),
	task{ 3, "watercontroller" },
	interval_clock{ this, 20 * bmptk::us, "interval" },
	temp_pool("temp"),
	temp_mutex("temp")
{}

void TempController::setTemp(int temp) {
	temp_mutex.wait();
	temp_pool.write(temp);
	temp_mutex.signal();
}

int TempController::getTemp() {
	temp_mutex.wait();
	int temp = temp_pool.read();
	temp_mutex.signal();
	return temp;
}

void TempController::main() {
	for (;;) {
		wait(interval_clock);
		int newtemp = getTemp();
		int temp = tempsensor.getTemp();
		if (temp >= newtemp) {
			heater.off();
			wascontroller.setTempReached();	// flag zetten
		}
		else heater.on();
	}
}

