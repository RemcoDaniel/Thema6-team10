/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

#include "DoorSoapController.h"

DoorSoapController::DoorSoapController(Door & door, SoapDispenser & soap, WashingMachineController & wascontroller) :
	door{ door },
	soap{ soap },
	wascontroller{ wascontroller },
	task{ 0, "motorctrl" },		// priority, name
	response_flag{ this, "uart_response_ready" },
	new_job_flag{ this, "new_job" },
	motor_job_pool{ "motor_job" },
	motor_job_mutex{ "motor_job" },
	motor_time_pool{ "motor_time" },
	motor_time_mutex{ "motor_time" },
	response_pool{ "uart_response" },
	response_mutex{ "uart_response" },
	rotate_timer{ this, "rotate_timer" }
{}

void DoorSoapController::stopMotor() {
	char * command = motor.turn(0, 0);
	char * response = uartTask(command);
	if (command[1] != response[1]) {		// antwoord moet zelfde zijn als request. niet zo? --> melding maken
											// melding moet hier komen...
	}
}


// UART =======================================================================================================================
void DoorSoapController::setResponseFlag() {
	response_flag.set();
}

void DoorSoapController::writeResponse(char * response) {
	response_mutex.wait();
	response_pool.write(response);
	response_mutex.signal();
}

char * DoorSoapController::readResponse() {
	response_mutex.wait();
	char * response = response_pool.read();
	response_mutex.signal();
	return response;
}

char * DoorSoapController::uartTask(char * command) {
	uart.writeChannel(command);
	wait(response_flag);
	return readResponse();			// pool uitlezen
}


// MAIN =======================================================================================================================
void DoorSoapController::main() {
	for (;;) {

	}
}

