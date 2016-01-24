// file MotorController.cpp

#include "MotorController.h"

MotorController::MotorController(WashingMachineController * wascontroller) :
	wascontroller{wascontroller},
	uartptr{nullptr},
	task{ 0, "motorctrl" },		// priority, name
	response_flag{ this, "uart_response_ready" },
	start_flag{ this, "start_motorcontroller" },
	new_job_flag{ this, "new_job" },
	motor_job_pool{"motor_job" },
	motor_job_mutex{"motor_job" },
	motor_time_pool{"motor_time"},
	motor_time_mutex{ "motor_time" },
	response_pool{ "uart_response" },
	response_mutex{ "uart_response" },
	rotate_timer{ this, "rotate_timer" }
{
	motor = Motor();
}

void MotorController::setUartPointer(UART * u) {
	uartptr = u;
}

void MotorController::startMotorController() {
	std::cout << "motorcontroller started\n";
	start_flag.set();
}

void MotorController::stopMotor() {
	command = motor.turn(1, 0);
	char response = uartTask(command[0], command[1]);
	if (command[1] != response) {		// antwoord moet zelfde zijn als request. niet zo? --> melding maken
		 // melding moet hier komen...
	}
}

void MotorController::rotateLeft(int speed) {
	command = motor.turn(1, speed);
	char response = uartTask(command[0], command[1]);
	if (command[1] != response) {		// antwoord moet zelfde zijn als request. niet zo? --> melding maken
											// melding moet hier komen...
	}
}

void MotorController::rotateRight(int speed) {
	command = motor.turn(0, speed);
	char response = uartTask(command[0], command[1]);
	if (command[1] != response) {		// antwoord moet zelfde zijn als request. niet zo? --> melding maken
											// melding moet hier komen...
	}
}

void MotorController::setMotorJob(int job, unsigned long int time) {
	// eerst de job erin zetten:
	motor_job_mutex.wait();
	motor_job_pool.write(job);
	motor_job_mutex.signal();

	// nu nog de tijd zetten:
	motor_time_mutex.wait();
	motor_time_pool.write(time);
	motor_time_mutex.signal();

	new_job_flag.set();
}

void MotorController::normalMotorJob(unsigned long int time) {
	speed = 500;
	for (int i = 0; i < 4; i++) {
		if (i == 2) {
			speed = 1000;
		}
		std::cout << "links draaien\n";
		rotateLeft(speed);
		rotate_timer.set(time);			// eigenlijk / 2 / 4
		wait(rotate_timer);

		std::cout << "rechts draaien\n";
		rotateRight(speed);
		rotate_timer.set(time);
		wait(rotate_timer);
	}
	stopMotor();
	std::cout << "motor gestopt\n";
	wascontroller->setMotorDone();	//	flag zetten
}

void MotorController::centrifuge(unsigned long int time) {
	int speed = 100;
	for (int i = 1; i < 16; i++) {
		rotateLeft(speed * i);
		rotate_timer.set((time / 2) / 16);
		wait(rotate_timer);
	}
	speed = 100;
	for (int i = 1; i < 16; i++) {
		rotateRight(speed * i);
		rotate_timer.set((time / 2) / 16);
		wait(rotate_timer);
	}
	stopMotor();
	std::cout << "motor gestopt\n";
	wascontroller->setMotorDone();	//	flag zetten
}

void MotorController::startMotorJob() {
	// job lezen
	motor_job_mutex.wait();
	job = motor_job_pool.read();
	motor_job_mutex.signal();

	// tijd lezen
	motor_time_mutex.wait();
	time = motor_time_pool.read();
	motor_time_mutex.signal();

	switch (job) {
		case 0: stopMotor(); break;
		case 1:	normalMotorJob(time); break;
		case 2: centrifuge(time); break;
	}
}

int MotorController::getMotorSpeed() {
	command = motor.getMotorSpeedCommand();
	char response = uartTask(command[0], command[1]);
	int speed = 0;		// nog iets doen met het antwoord, omzetten naar een speed;
	return speed;
}


// UART =======================================================================================================================
void MotorController::setResponseFlag() {
	response_flag.set();
}

void MotorController::writeResponse(char response) {
	response_mutex.wait();
	response_pool.write(response);
	response_mutex.signal();
}

char MotorController::readResponse() {
	response_mutex.wait();
	char response = response_pool.read();
	response_mutex.signal();
	return response;
}

char MotorController::uartTask(char request, char command) {
	uartptr->writeChannel(request, command);
	wait(response_flag);
	return readResponse();			// pool uitlezen
}


// MAIN =======================================================================================================================
void MotorController::main() {
	wait(start_flag);
	for (;;) {
		wait(new_job_flag);				// checken of er een nieuwe taak is
		startMotorJob();				// deze job uit gaan voeren
	}
}

