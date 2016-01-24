// file WebController.cpp

#include "WebController.h"

WebController::WebController(WasmachineApp * app, WashingMachineController * was, MotorController * motor, TempController * temp, WaterController * water) :
	app{app},
	wasctrl{was},
	motorctrl{motor},
	tempctrl{temp},
	waterctrl{water},
	task{ 1, "webctrl" },		// priority, name
	interval_clock{ this, 1 S, "interval" },
	temp_pool{ "temp" },
	temp_mutex{ "temp" },
	water_pool{ "water" },
	water_mutex{ "water" },
	motor_pool{ "motor" },
	motor_mutex{ "motor" },
	status_pool{ "status" },
	status_mutex{ "status" }
{
	status = WACHTEN;
}

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

// STATUS ======================================================================================================================
void WebController::setStatus(int status) {
	status_mutex.wait();
	status_pool.write(status);
	status_mutex.signal();
}

void WebController::getStatus() {
	status_mutex.wait();
	status = status_pool.read();
	status_mutex.signal();
}

// AFHANDELING ================================================================================================================
void WebController::logging() {
	// hier moet hij de temp, waterlevel en motorrpm opvragen en naar de andere kant sturen
	//WasmachineApp::broadcastMessage(const string & msg);
	//app->broadcastMessage(const string & msg);
}

void WebController::sendStatus() {
	getStatus();
	switch (status) {
		case WACHTEN: 		app->broadcastMessage("wachten op deur");	break;
		case STARTEN: 		app->broadcastMessage("starten");			break;
		case VOORWAS: 		app->broadcastMessage("voorwas");			break;
		case HOOFDWAS: 		app->broadcastMessage("hoofdwas");			break;
		case SPOELEN: 		app->broadcastMessage("spoelen");			break;
		case CENTRIFUGE: 	app->broadcastMessage("centrifugeren");		break;
		case KLAAR: 		app->broadcastMessage("klaar");				break;
	}
}

void WebController::messageHandling() {
	if(app->isMsg()) {
		wasprogrammaStruct wasstruct = app->getLastMsg();
		std::cout << wasstruct.temperature << " dit is temp\n";
		Wasprogramma * was = new Wasprogramma(wasstruct.temperature, wasstruct.waterlevel, wasstruct.time, wasstruct.job);
		wasctrl->setProgram(was); // wasmachinecontroller het wasprogramma geven
	}
}

// MAIN =======================================================================================================================
void WebController::main() {
	for (;;) {
		wait(interval_clock);
		messageHandling();
		sendStatus();
		//logging();			// moet dit altijd, of alleen als erom wordt gevraagd?
	}
}