/**
 * @file      Heater.cpp
 * @version   0.1
 * @author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * @date      19-01-2016
 */

TempController::TempController(WashingMachineController * wascontroller) :
	wascontroller{wascontroller},
	uartptr{nullptr},
	task{ 2, "tempcontroller" },
	interval_clock{ this, 300 MS, "interval" },
	response_flag{this, "uart_response_ready"},
	start_flag{ this, "start_tempcontroller" },
	temp_pool{"temp"},
	temp_mutex{"temp"},
	response_pool{"uart_response"},
	response_mutex{"uart_response"}
{
	heater = Heater();
	tempsensor = TempSensor();
}

void TempController::setUartPointer(UART * u) {
	uartptr = u;
}

void TempController::startTempController() {
	std::cout << "tempcontroller started\n";
	start_flag.set();
}

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
	command = tempsensor.getTempCommand();
	char response = uartTask(command[0], command[1]);
	int temp = response;
	return temp;
}

void TempController::heat(bool on) {
	if (on) {
		command = heater.getOnCommand();
	}
	else {
		command = heater.getOffCommand();
	}
	char response = uartTask(command[0], command[1]);
}

// UART =======================================================================================================================
char TempController::readResponse() {
	response_mutex.wait();
	char response = response_pool.read();
	response_mutex.signal();
	return response;
}

char TempController::uartTask(char request, char command) {
	uartptr->writeChannel(request, command);
	wait(response_flag);
	return readResponse();
}

void TempController::setResponseFlag() {
	response_flag.set();
}

void TempController::writeResponse(char response) {
	response_mutex.wait();
	response_pool.write(response);
	response_mutex.signal();
}


// MAIN =======================================================================================================================
void TempController::main() {
	wait(start_flag);
	for (;;) {
		wait(interval_clock);
		int newtemp = getNewTemp();
		int temp = getTemp();

		if (temp >= newtemp) {
			heat(0);							// heater uit
			wascontroller->setTempReached();
		}
		else {
			heat(1);							// heater aan
		}
	}
}

