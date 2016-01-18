#include "TempController.h"

TempController::TempController(Heater & heater, TempSensor & tempsensor, WashingMachineController & wascontroller, UART & uart) :
	heater{heater},
	tempsensor{tempsensor},
	wascontroller{wascontroller},
	uart{uart},
	task{ 3, "watercontroller" },
	interval_clock{ this, 20 * bmptk::us, "interval" },
	response_flag{this, "uart_response_ready"},
	temp_pool{"temp"},
	temp_mutex{"temp"},
	response_pool{"uart_response"},
	response_mutex{"uart_response"}
{}

void TempController::setTemp(int temp) {
	temp_mutex.wait();
	temp_pool.write(temp);
	temp_mutex.signal();
}

int TempController::getNewTemp() {
	temp_mutex.wait();
	int temp = temp_pool.read();
	temp_mutex.signal();
	return temp;
}

int TempController::getTemp() {
	char * command = tempsensor.getTempCommand();
	char * response = uartTask(command);
	int temp = response[1];
	return temp;
}

void TempController::heat(bool on) {
	char * command;
	if (on) {
		command = heater.getOnCommand();
	}
	command = heater.getOffCommand();
	char * response = uartTask(command);
}

// UART =======================================================================================================================
char * TempController::readResponse() {
	response_mutex.wait();
	char * response = response_pool.read();
	response_mutex.signal();
	return response;
}

char * TempController::uartTask(char * command) {
	uart.executeCommand(command);
	wait(response_flag);
	return readResponse();			// pool uitlezen
}

void TempController::setResponseFlag() {
	response_flag.set();
}

void TempController::writeResponse(char * response) {
	response_mutex.wait();
	response_pool.write(response);
	response_mutex.signal();
}


// MAIN =======================================================================================================================
void TempController::main() {
	for (;;) {
		wait(interval_clock);
		int newtemp = getNewTemp();
		int temp = getTemp();
		if (temp >= newtemp) {
			heat(0);						// heater uit
			wascontroller.setTempReached();	// flag zetten
		}
		else heat(1);						// heater aan
	}
}

