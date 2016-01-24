/**
 * \class The TempController controller class
 *
 * Makes sure that the temprature is right.
 *
 */
 
#ifndef _TEMPCONTROLLER_H
#define _TEMPCONTROLLER_H

#include "Heater.h"
#include "TempSensor.h"
#include "WashingMachineController.h"
#include "pRTOS.h"
#include "UART.h"

class WashingMachineController;
class UART;

class TempController : public RTOS::task {
private:
	char * command = 0;
	Heater heater;
	TempSensor tempsensor;
	UART *uartptr;
	WashingMachineController *wascontroller;
	RTOS::flag response_flag, start_flag;
	RTOS::pool< int > temp_pool;
	RTOS::pool< char > response_pool;
	RTOS::mutex temp_mutex, response_mutex;
	RTOS::clock interval_clock;

	int getNewTemp();
	int getTemp();
	void heat(bool on);		// on = 1 , off = 0

	//uart:
	char readResponse();
	char uartTask(char request, char command);

public:
	TempController(WashingMachineController * wascontroller);
	
	/*! \fn void setUartPointer(UART *u)
	 *  \brief Used to (re)set the pointer to the uart.
	 *  \param u* pointer to the UART object this controller should use.
	 */
	void setUartPointer(UART * u);

	/*!	\fn void setTemp(int temp)
	 *	\brief sets the temprature of the emulator
	 *	\param temp in celcius degrees
	 *	\return void
	 */
	void setTemp(int temp);
	
	//uart:
	/*!	\fn void setResponseFlag()
	 *	\brief sets the response flag
	 *	\return void
	 */
	void setResponseFlag();
	 
	/*!	\fn void writeResponse(char response)
	 *	\brief writes a response in the response pool 
	 *	\param response a char array with two positions
	 *	\return void
	 */
	void writeResponse(char response);
	 
	void startTempController();

	void main();
};

#endif