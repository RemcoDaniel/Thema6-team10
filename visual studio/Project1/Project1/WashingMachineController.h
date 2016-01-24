/**
 * \class The WashingMachineController controller class
 */
 
#pragma once
#ifndef _WASHINGMACHINECONTROLLER_H
#define _WASHINGMACHINECONTROLLER_H

#include "pRTOS.h"
#include "MotorController.h"
#include "WaterController.h"
#include "TempController.h"
#include "Door.h"
#include "Wasprogramma.h"
#include "UART.h"
#include "SoapDispenser.h"
#include "WebController.h"
#include "WasmachineApp.h"

class UART;
class MotorController;
class WaterController;
class TempController;
class WebController;
class WasmachineApp;

class WashingMachineController : public RTOS::task {
private:
	WasmachineApp app;
	char * command = 0;
	Wasprogramma * wasprogramma;
	Door door;
	TempController *tempcontroller;
	WaterController *watercontroller;
	MotorController *motorcontroller;
	WebController * webcontroller;
	UART *uart;
	SoapDispenser soap;
	RTOS::flag temp_reached_flag, level_reached_flag, motor_done_flag, response_flag, program_ready_flag;
	RTOS::pool< char > response_pool;
	RTOS::pool< Wasprogramma * > program_pool;
	RTOS::mutex response_mutex, program_mutex;
	RTOS::clock interval_clock;

	void doorlock(bool lock);		// lock = 1 , unlock = 0
	bool getDoorStatus();			// open = 1, locked or closed = 0;
	void dispendSoap();
	void signalLed(bool on);

	void getProgram();
	
	//uart:
	char readResponse();
	char uartTask(char request, char command);

public:

	WashingMachineController(WasmachineApp app);
	/*! \fn WashingMachineController(Wasprogramma & was)
	 *	\brief Creates the controller and assignes a washingschedule
	 *	\return void
	 */

	void setTempReached();
	/*! \fn void setTempReached()
	 *	\brief Sets the temprature as reached
	 *	\return void
	 */
	void setWaterLevelReached();
	/*! \fn void setWaterLevelReached()
	 *	\brief Sets the waterlevel as reached
	 *	\return void
	 */
	void setMotorDone();
	/*! \fn void setMotorDone()
	 *	\brief Sets the motor as done with the job
	 *	\return void
	 */

	void startWasprogramma();
	/*! \fn void startWasprogramma()
	 *	\brief Starts the washing schedule
	 *	\return void
	 */
	void stopWasprogramma();
	/*! \fn void stopWasprogramma()
	 *	\brief Stops the washing schedule
	 *	\return void
	 */

	//uart:
	void setResponseFlag();
	/*!	\fn void setResponseFlag()
	 *	\brief sets the response flag
	 *	\return void
	 */
	 
	void writeResponse(char response);
	/*!	\fn void writeResponse(char * response)
	 *	\brief writes a response in the response pool 
	 *	\param response a char array with two positions
	 *	\return void
	 */
	 
	void setProgram(Wasprogramma * was);

	void main();
};

#endif