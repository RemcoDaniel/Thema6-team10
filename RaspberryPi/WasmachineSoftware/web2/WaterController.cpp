#include "WaterController.h"

WaterController::WaterController(WashingMachineController * wascontroller) :
	wascontroller{wascontroller},
	uartptr{nullptr},
	task{3, "watercontroller"},
	interval_clock{this, 300 MS, "interval"},
	response_flag{this, "uart_response_ready"},
	start_flag{ this, "start_waterController" },
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

void WaterController::startWaterController() {
	start_flag.set();
}

int WaterController::getNewWaterLevel() {
	water_level_mutex.wait();
	int level = water_level_pool.read();
	water_level_mutex.signal();
	return level;
}

int WaterController::getWaterLevel() {
	command = watersensor.getWaterLevelCommand();
	char response = uartTask(command[0], command[1]);
	return response;
}

void WaterController::setWaterLevel(int level) {
	water_level_mutex.wait();
	water_level_pool.write(level);
	water_level_mutex.signal();
}

void WaterController::pumping(bool on) {
	if (on) {
		command = pump.getOnCommand();
	}
	else {
		command = pump.getOffCommand();
	}
	char response = uartTask(command[0], command[1]);
}

void WaterController::valving(bool on) {
	char * command;
	if (on) {
		command = valve.getOnCommand();
	}
	else {
		command = valve.getOffCommand();
	}
	char response = uartTask(command[0], command[1]);
}


// UART =======================================================================================================================
char WaterController::readResponse() {
	response_mutex.wait();
	char response = response_pool.read();
	response_mutex.signal();
	return response;
}

char WaterController::uartTask(char request, char command) {
	uartptr->writeChannel(request, command);
	wait(response_flag);
	return readResponse();
}

void WaterController::setResponseFlag() {
	response_flag.set();
}

void WaterController::writeResponse(char response) {
	response_mutex.wait();
	response_pool.write(response);
	response_mutex.signal();
}


// MAIN =======================================================================================================================
void WaterController::main() {
	wait(start_flag);
	for (;;) {
		wait(interval_clock);
		int newlevel = getNewWaterLevel();
		int level = getWaterLevel();
		if (level >= newlevel) {
			valving(0);	
			if (level > newlevel) {
				pumping(1);		
			}
		}
		else {
			pumping(0);	
			valving(1);
		}
		newlevel = getNewWaterLevel();
		level = getWaterLevel();
		if(level == newlevel) {
			wascontroller->setWaterLevelReached();
		}
	}
}

