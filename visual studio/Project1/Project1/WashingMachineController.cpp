// file WashingMachineController.cpp

#include "WashingMachineController.h"

WashingMachineController::WashingMachineController(WasmachineApp * app) :
	app{app},
	wasprogramma{nullptr},
	task{0, "beep"},
	interval_clock{this, 500 MS, "washing_timer"},
	temp_reached_flag{this, "temp_reached"},
	level_reached_flag{this, "water_reached"},
	response_flag{this, "uart_response_ready"},
	program_ready_flag{this, "washingprogram_ready"},
	motor_done_flag{this, "motor_done"},
	response_pool{"uart_response"},
	response_mutex{"uart_response"},
	program_pool{"washing_program"},
	program_mutex{"washing_program"}
{
	// boundaries:
	soap = SoapDispenser();
	door = Door();

	//controllers aanmaken:
	motorcontroller = new MotorController(this);
	tempcontroller = new TempController(this);
	watercontroller = new WaterController(this);

	// uart aanmaken:
	uart = new UART("/dev/ttyAMA0", 9600, motorcontroller, tempcontroller, watercontroller, this);

	webcontroller = new WebController(app, this, motorcontroller, tempcontroller, watercontroller);
	
	// nu de controllers een shared pointer geven van de uart:
	motorcontroller->setUartPointer(uart);
	tempcontroller->setUartPointer(uart);
	watercontroller->setUartPointer(uart);
}

bool WashingMachineController::getDoorStatus() {
	char command[3] = { DOOR_LOCK_REQ , STATUS_CMD, '\0' };
	char response = uartTask(command[0], command[1]);
	int status = unsigned(response);

	if (status == OPENED) {
		return 0;
	}
	else {
		return 1;
	}
	return status;
}

void WashingMachineController::doorlock(bool lock) {
	if (lock) {
		char response = uartTask(DOOR_LOCK_REQ, LOCK_CMD);
	}
	else {
		char response = uartTask(DOOR_LOCK_REQ, UNLOCK_CMD);
	}
}

void WashingMachineController::signalLed(bool on) {
	if (on) {
		char response = uartTask(SIGNAL_LED_REQ, ON_CMD);
	}
	else {
		char response = uartTask(SIGNAL_LED_REQ, OFF_CMD);
	}
}

void WashingMachineController::dispendSoap() {
	command = soap.getOpenCommand();
	char response = uartTask(command[0], command[1]);
	
	command = soap.getCloseCommand();
	response = uartTask(command[0], command[1]);
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

void WashingMachineController::setProgram(Wasprogramma * was) {
	program_mutex.wait();
	program_pool.write(was);
	program_mutex.signal();
	program_ready_flag.set();
}

// WASPROGRAMMA ============================================================================================
void WashingMachineController::getProgram() {
	program_mutex.wait();
	wasprogramma = program_pool.read();
	program_mutex.signal();
}

void WashingMachineController::startWasprogramma() {
	webcontroller->setStatus(STARTEN);												// sending status
	signalLed(0);																	// signaalled uit

	// voorwas (zonder zeep!):
	std::cout << "---START VOORWAS---\n";
	webcontroller->setStatus(VOORWAS);												// sending status
	std::cout << "water toevoegen\n";
	watercontroller->startWaterController();
	watercontroller->setWaterLevel(wasprogramma->getLevel() / 2);					// water erbij
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	std::cout << "verwarmen\n";
	tempcontroller->startTempController();
	tempcontroller->setTemp(wasprogramma->getTemp());								// verwarmen
	wait(temp_reached_flag);														// wachten tot temp is bereikt
	std::cout << "draaien\n";
	motorcontroller->startMotorController();
	motorcontroller->setMotorJobTime(wasprogramma->getTime());						// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	std::cout << "water wegpompen\n";
	watercontroller->setWaterLevel(0);												// water weg
	wait(level_reached_flag);														
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	//hoofdwas:
	std::cout << "---START HOOFDWAS---\n";
	webcontroller->setStatus(HOOFDWAS);												// sending status
	std::cout << "water toevoegen\n";
	watercontroller->setWaterLevel(wasprogramma->getLevel());						// water erbij
	wait(level_reached_flag);														
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	std::cout << "verwarmen\n";
	tempcontroller->setTemp(wasprogramma->getTemp());								// verwarmen
	wait(temp_reached_flag);														// wachten tot temp is bereikt
	std::cout << "zeep toevoegen\n";
	dispendSoap();																	// zeep erbij
	std::cout << "draaien\n";
	motorcontroller->setMotorJobTime(wasprogramma->getTime());						// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	std::cout << "water wegpompen\n";
	watercontroller->setWaterLevel(0);												// water weg
	wait(level_reached_flag);														
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	// spoelen:
	std::cout << "---START SPOELEN---\n";
	webcontroller->setStatus(SPOELEN);												// sending status
	std::cout << "water toevoegen\n";
	watercontroller->setWaterLevel(wasprogramma->getLevel() / 2);					// water erbij
	wait(level_reached_flag);														
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	std::cout << "draaien\n";
	motorcontroller->setMotorJobTime(wasprogramma->getTime());						// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	std::cout << "water wegpompen en heater uitzetten\n";
	watercontroller->setWaterLevel(0);												// water weg
	tempcontroller->setTemp(0);														// verwarming uitzetten
	wait(level_reached_flag);														
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	// centrifugeren:
	std::cout << "---START CENTRIFUGEREN---\n";
	webcontroller->setStatus(CENTRIFUGE);											// sending status
	std::cout << "draaien\n";
	motorcontroller->setMotorJobTime(wasprogramma->getTime());						// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	std::cout << "deur ontgrendelen\n";
	doorlock(0);																	// deur ontgrendelen
	signalLed(1);																	// signaalled aan
	std::cout << "---WAS IS KLAAR---\n";
	webcontroller->setStatus(KLAAR);												// sending status
}


// UART =======================================================================================================================
char WashingMachineController::readResponse() {
	response_mutex.wait();
	char response = response_pool.read();
	response_mutex.signal();
	return response;
}

char WashingMachineController::uartTask(char request, char command) {
	uart->writeChannel(request, command);
	wait(response_flag);
	return readResponse();
}

void WashingMachineController::setResponseFlag() {
	response_flag.set();
}

void WashingMachineController::writeResponse(char response) {
	response_mutex.wait();
	response_pool.write(response);
	response_mutex.signal();
}


// MAIN =======================================================================================================================
void WashingMachineController::main() {
	for (;;) {
		wait(program_ready_flag);
		getProgram();

		wait(response_flag);						// wachten op signaal van de uart dat machine gestart is:
		
		webcontroller->setStatus(WACHTEN);			// sending status
		while (!getDoorStatus()) {					// wachten tot deur wasmachine is gesloten
			wait(interval_clock);
		}
		doorlock(1);
		
		startWasprogramma();
	}
}