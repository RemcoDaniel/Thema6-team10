// file WashingMachineController.cpp

#include "WashingMachineController.h"

WashingMachineController::WashingMachineController(Door & door, TempController & temp, WaterController & water, MotorController & motor, Wasprogramma & was, UART & uart, SoapDispenser & soap) :
	door{door},
	tempcontroller{temp},
	watercontroller{water},
	motorcontroller{motor},
	wasprogramma{was},
	uart{uart},
	soap{soap},
	task{0, "beep"},			// priority instellen nog !!!!!!!!!
	interval_clock{this, 500 * bmptk::us, "washing_timer"},
	temp_reached_flag{this, "temp_reached"},
	level_reached_flag{this, "water_reached"},
	response_flag{this, "uart_response_ready"},
	motor_done_flag{this, "motor_done"},
	response_pool{"uart_response"},
	response_mutex{"uart_response"}
{}

void WashingMachineController::doorlock(bool lock) {
	char * command;
	if (lock) {
		command = door.getLockCommand();
	}
	command = door.getUnlockCommand();
	char * response = uartTask(command);
}

void WashingMachineController::dispendSoap() {
	char * command;
	command = soap.getOpenCommand();
	command = soap.getCloseCommand();

	char * response = uartTask(command);
}

// FLAGS ===================================================================================================
void WashingMachineController::setTempReached() {
	temp_reached_flag.set();
}

void WashingMachineController::setWaterLevelReached() {
	level_reached_flag.set();
}

void WashingMachineController::setMotorDone() {
	motor_done_flag.set();
}


// WASPROGRAMMA ============================================================================================
void WashingMachineController::startWasprogramma() {
	doorlock(1);																	// deur dicht

	// voorwas (zonder zeep!):
	watercontroller.setWaterLevel(wasprogramma.getLevel());							// water erbij
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	tempcontroller.setTemp(wasprogramma.getTemp());									// verwarmen
	wait(temp_reached_flag);														// wachten tot temp is bereikt
	motorcontroller.setMotorJob(wasprogramma.getJob(), wasprogramma.getTime());		// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	watercontroller.setWaterLevel(0);												// water weg
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	//hoofdwas:
	watercontroller.setWaterLevel(wasprogramma.getLevel());							// water erbij
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	tempcontroller.setTemp(wasprogramma.getTemp());									// verwarmen
	wait(temp_reached_flag);														// wachten tot temp is bereikt
	dispendSoap();																	// zeep erbij
	motorcontroller.setMotorJob(wasprogramma.getJob(), wasprogramma.getTime());		// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	watercontroller.setWaterLevel(0);												// water weg
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	// spoelen:
	watercontroller.setWaterLevel(wasprogramma.getLevel());							// water erbij
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	motorcontroller.setMotorJob(wasprogramma.getJob(), wasprogramma.getTime());		// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	watercontroller.setWaterLevel(0);												// water weg
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	// centrifugeren:
	motorcontroller.setMotorJob(wasprogramma.getJob(), wasprogramma.getTime());		// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is

	doorlock(0);																	// deur ontgrendelen
}

void WashingMachineController::stopWasprogramma() {

}


// UART =======================================================================================================================
char * WashingMachineController::readResponse() {
	response_mutex.wait();
	char * response = response_pool.read();
	response_mutex.signal();
	return response;
}

char * WashingMachineController::uartTask(char * command) {
	uart.executeCommand(command);
	wait(response_flag);
	return readResponse();			// pool uitlezen
}

void WashingMachineController::setResponseFlag() {
	response_flag.set();
}

void WashingMachineController::writeResponse(char * response) {
	response_mutex.wait();
	response_pool.write(response);
	response_mutex.signal();
}


// MAIN =======================================================================================================================
void WashingMachineController::main() {
	for (;;) {
		// wachten op signaal voor nieuw wasprogramma
		startWasprogramma();		// wasprogramma starten
		// kan ondertussen signaal krijgen dat wasprogramma onderbroken moet worden!!!!!!!!!!!!!
	}
}