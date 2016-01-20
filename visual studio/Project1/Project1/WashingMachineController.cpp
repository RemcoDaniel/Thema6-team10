// file WashingMachineController.cpp

#include "WashingMachineController.h"

WashingMachineController::WashingMachineController(Wasprogramma & was) :
	wasprogramma{was},
	task{0, "beep"},
	interval_clock{this, 500 US, "washing_timer"},
	temp_reached_flag{this, "temp_reached"},
	level_reached_flag{this, "water_reached"},
	response_flag{this, "uart_response_ready"},
	motor_done_flag{this, "motor_done"},
	response_pool{"uart_response"},
	response_mutex{"uart_response"}
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

	// nu de controllers een shared pointer geven van de uart:
	motorcontroller->setUartPointer(uart);
	tempcontroller->setUartPointer(uart);
	watercontroller->setUartPointer(uart);

	std::cout << "wasmachine constructor done\n";
}

bool WashingMachineController::getDoorStatus() {
	std::cout << "getting door status\n";
	char command[3] = { DOOR_LOCK_REQ , STATUS_CMD, '\0' };
	char response = uartTask(command[0], command[1]);
	std::cout << "response door binnen\n";
	int status = unsigned(response);

	if (status == OPENED) {
		return 0;
	}
	else {
		return 1;
	}
	std::cout << "got status\n";
	return status;
}

void WashingMachineController::doorlock(bool lock) {
	//char * command = door.getUnlockCommand();;
	if (lock) {
		//command = door.getLockCommand();
		char response = uartTask(DOOR_LOCK_REQ, LOCK_CMD);
	}
	else {
		char response = uartTask(DOOR_LOCK_REQ, UNLOCK_CMD);
	}
	std::cout << "lock/unlock door\n";
	//std::cout << unsigned(command[0]) << " " << unsigned(command[1]) << " dit staat er dus :D\n";
	//char response = uartTask(command[0], command[1]);
}

void WashingMachineController::dispendSoap() {
	char * command;
	command = soap.getOpenCommand();
	command = soap.getCloseCommand();

	char response = uartTask(command[0], command[1]);
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
	std::cout << "0\n";

	// voorwas (zonder zeep!):
	std::cout << "1\n";
	watercontroller->setWaterLevel(wasprogramma.getLevel());						// water erbij
	std::cout << "voor de flag\n";
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	std::cout << "na de flag\n";
	tempcontroller->setTemp(wasprogramma.getTemp());								// verwarmen
	wait(temp_reached_flag);														// wachten tot temp is bereikt

	//motorcontroller->setMotorJob(wasprogramma.getJob(), wasprogramma.getTime());	// draaien
	//wait(motor_done_flag);															// wachten tot motor klaar is
	//watercontroller->setWaterLevel(0);												// water weg
	//wait(level_reached_flag);														// wachten tot water weg is (flag)

	//hoofdwas:
	std::cout << "2\n";
	watercontroller->setWaterLevel(wasprogramma.getLevel());						// water erbij
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	tempcontroller->setTemp(wasprogramma.getTemp());								// verwarmen
	wait(temp_reached_flag);														// wachten tot temp is bereikt
	dispendSoap();																	// zeep erbij
	motorcontroller->setMotorJob(wasprogramma.getJob(), wasprogramma.getTime());	// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	watercontroller->setWaterLevel(0);												// water weg
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	// spoelen:
	std::cout << "3\n";
	watercontroller->setWaterLevel(wasprogramma.getLevel());						// water erbij
	wait(level_reached_flag);														// wachten tot voldoende water (flag)
	motorcontroller->setMotorJob(wasprogramma.getJob(), wasprogramma.getTime());	// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is
	watercontroller->setWaterLevel(0);												// water weg
	wait(level_reached_flag);														// wachten tot water weg is (flag)

	// centrifugeren:
	std::cout << "4\n";
	motorcontroller->setMotorJob(wasprogramma.getJob(), wasprogramma.getTime());	// draaien
	wait(motor_done_flag);															// wachten tot motor klaar is

	std::cout << "5\n";
	doorlock(0);																	// deur ontgrendelen
}

void WashingMachineController::stopWasprogramma() {

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
	std::cout << "washing machine read uart response\n";
	return readResponse();			// pool uitlezen
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
		std::cout << "starting washing program\n";
		// wachten op signaal voor nieuw wasprogramma
		// wachten op signaal van de uart dat machine gestart is:
		wait(response_flag);
		std::cout << "flag gezien\n";

		while (!getDoorStatus()) {
			std::cout << "door not closed!\n";
			wait(interval_clock);
		}
		doorlock(1);																	// deur dicht

		startWasprogramma();		// wasprogramma starten
		// kan ondertussen signaal krijgen dat wasprogramma onderbroken moet worden!!!!!!!!!!!!!
	}
}