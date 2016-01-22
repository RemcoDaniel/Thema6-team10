// file WebController.cpp

#include "WebController.h"

WebController::WebController(WasmachineApp app, WashingMachineController * was, MotorController * motor, TempController * temp, WaterController * water) :
	app{app},
	wasctrl{was},
	task{ 1, "webctrl" },		// priority, name
	interval_clock{ this, 1 S, "interval" },
	temp_pool{ "temp" },
	temp_mutex{ "temp" },
	water_pool{ "water" },
	water_mutex{ "water" },
	motor_pool{ "motor" },
	motor_mutex{ "motor" },
{}

// TEMP ======================================================================================================================
void WebController::setTemp(int temp) {
	temp_mutex.wait();
	temp_pool.write(temp);
	temp_mutex.signal();
}

void WebController::getTemp() {
	temp_mutex.wait();
	temp = temp_pool.read();
	temp_mutex.signal();
}

// WATER ======================================================================================================================
void WebController::setWaterLevel(int water) {
	water_mutex.wait();
	water_pool.write(water);
	water_mutex.signal();
}

void WebController::getWaterLevel() {
	water_mutex.wait();
	water = water_pool.read();
	water_mutex.signal();
}

// MOTOR ======================================================================================================================
void WebController::setMotorRPM(int motor) {
	motor_mutex.wait();
	motor_pool.write(motor);
	motor_mutex.signal();
}

void WebController::getMotorRPM() {
	motor_mutex.wait();
	motor = motor_pool.read();
	motor_mutex.signal();
}

// AFHANDELING ================================================================================================================
void WebController::logging() {
	// hier moet hij de temp, waterlevel en motorrpm opvragen en naar de andere kant sturen
}

void WebController::messageHandling() {
	if(app.isMsg()) {
		wasprogrammaStruct wasstruct = app.getLastMsg();
		Wasprogramma * was = new Wasprogramma(wasstruct.temperature, wasstruct.waterlevel, wasstruct.time, wasstruct.job);
		wasctrl.setProgram(was); // wasmachinecontroller het wasprogramma geven
	}
}

// MAIN =======================================================================================================================
void WebController::main() {
	for (;;) {
		wait(interval_clock);
		messageHandling();
		//logging();			// moet dit altijd, of alleen als erom wordt gevraagd?
	}
}