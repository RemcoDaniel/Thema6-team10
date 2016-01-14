// file WashingMachineController.cpp

#include "WashingMachineController.h"

WashingMachineController::WashingMachineController(TempController tempcontroller, WaterController watercontroller, MotorController motorcontroller) :
	tempcontroller(tempcontroller),
	watercontroller(watercontroller),
	motorcontroller(motorcontroller),
	task{ 0, "beep" },
	interval_clock{ this, 500 * bmptk::us, "washing_timer" },
	temp_reached_flag{ this, "temp_reached" },
	level_reached_flag{ this, "water_reached" },
	motor_done_flag{ this, "motor_done" }
{}


void WashingMachineController::setTempReached() {
	temp_reached_flag.set();
}

void WashingMachineController::setWaterLevelReached() {
	level_reached_flag.set();
}

void WashingMachineController::setMotorDone() {
	motor_done_flag.set();
}

void WashingMachineController::registerObserver() {

}

void WashingMachineController::removeObserver() {

}

void WashingMachineController::notifyObserver() {

}

void WashingMachineController::loadWasprogramma(int temp, int water, int time) {

}

void WashingMachineController::startWasprogramma(Wasprogramma wp) {
	// deur dicht

	// voorwas (zonder zeep!):
	watercontroller.setWaterLevel(wp.getLevel());		// water erbij
	wait(level_reached_flag);		// wachten tot voldoende water (flag)
	tempcontroller.setTemp(wp.getTemp());		// verwarmen
	wait(temp_reached_flag);		// wachten tot temp is bereikt
	motorcontroller.setMotorJob(wp.getJob());		// draaien
	wait(motor_done_flag);		// wachten tot motor klaar is
	watercontroller.setWaterLevel(0);		// water weg
	wait(level_reached_flag);		// wachten tot water weg is (flag)

	//hoofdwas:
	watercontroller.setWaterLevel(wp.getLevel());		// water erbij
	wait(level_reached_flag);		// wachten tot voldoende water (flag)
	tempcontroller.setTemp(wp.getTemp());		// verwarmen
	wait(temp_reached_flag);		// wachten tot temp is bereikt
	// zeep erbij
	motorcontroller.setMotorJob(wp.getJob());		// draaien
	wait(motor_done_flag);		// wachten tot motor klaar is
	watercontroller.setWaterLevel(0);		// water weg
	wait(level_reached_flag);		// wachten tot water weg is (flag)

	// spoelen:
	watercontroller.setWaterLevel(wp.getLevel());		// water erbij
	wait(level_reached_flag);		// wachten tot voldoende water (flag)
	motorcontroller.setMotorJob(wp.getJob());		// draaien
	wait(motor_done_flag);		// wachten tot motor klaar is
	watercontroller.setWaterLevel(0);		// water weg
	wait(level_reached_flag);		// wachten tot water weg is (flag)

	// centrifugeren:
	motorcontroller.setMotorJob(wp.getJob());		// draaien
	wait(motor_done_flag);		// wachten tot motor klaar is

	// deur ontgrendelen
}

void WashingMachineController::stopWasprogramma() {

}

void WashingMachineController::main() {
	for (;;) {
		// iets doen
		// wachten op signaal voor nieuw wasprogramma
		// wasprogramma starten
		// kan ondertussen signaal krijgen dat wasprogramma onderbroken moet worden!
	}
}