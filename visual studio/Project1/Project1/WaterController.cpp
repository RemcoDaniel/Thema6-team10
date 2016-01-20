/**
 * @file      Heater.cpp
 * @version   0.1
 * @author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * @date      19-01-2016
 */

#include "WaterController.h"

WaterController::WaterController(WashingMachineController * wascontroller) :
	wascontroller{wascontroller},
	uartptr{nullptr},
	task{3, "watercontroller"},
	interval_clock{this, 500 US, "interval"},
	response_flag{this, "uart_response_ready"},
	water_level_pool{"water_level"},
	water_level_mutex{"water_level"},
	response_pool{"uart_response"},
	response_mutex{"uart_response"}
{
	pump = Pump();
	valve = Valve();
	watersensor = WaterSensor();
}

void WaterController::setUartPointer(UART * u) {
	uartptr = u;
}

int WaterController::getNewWaterLevel() {
	water_level_mutex.wait();
	int level = water_level_pool.read();
	water_level_mutex.signal();
	return level;
}

int WaterController::getWaterLevel() {
	char * command = watersensor.getWaterLevelCommand();
	char * response = uartTask(command);
	int level = response[1];
	return level;
}

void WaterController::setWaterLevel(int level) {
	water_level_mutex.wait();
	water_level_pool.write(level);
	water_level_mutex.signal();
}

void WaterController::pumping(bool on) {
	char * command;
	if (on) {
		command = pump.getOnCommand();
	}
	command = pump.getOffCommand();
	char * response = uartTask(command);
}

void WaterController::valving(bool on) {
	char * command;
	if (on) {
		command = valve.getOnCommand();
	}
	command = valve.getOffCommand();
	char * response = uartTask(command);
}


// UART =======================================================================================================================
char * WaterController::readResponse() {
	response_mutex.wait();
	char * response = response_pool.read();
	response_mutex.signal();
	return response;
}

char * WaterController::uartTask(char * command) {
	uartptr->writeChannel(command);
	wait(response_flag);
	return readResponse();			// pool uitlezen
}

void WaterController::setResponseFlag() {
	response_flag.set();
}

void WaterController::writeResponse(char * response) {
	response_mutex.wait();
	response_pool.write(response);
	response_mutex.signal();
}


// MAIN =======================================================================================================================
void WaterController::main() {
	for (;;) {
		wait(interval_clock);
		int newlevel = getNewWaterLevel();
		int level = getWaterLevel();
		if (level >= newlevel) {
			pumping(0);					//pumping off
			if (level > newlevel) {
				wascontroller->setWaterLevelReached();
				valving(1);				// nog ff kijken naar het gedrag van de valve, want dit klopt niet helemaal...		// open valve
			}
		}
		else valving(0); pumping(1);	// close valve	// pumping on
	}
}

