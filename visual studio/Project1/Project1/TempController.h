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
class WashingMachineController;
#include "pRTOS.h"
#include "UART.h"

class TempController : public RTOS::task {
private:
	Heater heater;
	TempSensor tempsensor;
	UART *uartptr;
	WashingMachineController *wascontroller;
	RTOS::flag response_flag;
	RTOS::pool< int > temp_pool;
	RTOS::pool< char * > response_pool;
	RTOS::mutex temp_mutex, response_mutex;
	RTOS::clock interval_clock;

	int getNewTemp();
	int getTemp();
	void heat(bool on);		// on = 1 , off = 0

	//uart:
	char* readResponse();
	char* uartTask(char * command);

public:
	TempController(WashingMachineController * wascontroller);
	void setUartPointer(UART * u);
	/*! \fn void setUartPointer(UART *u)
	 *  \brief Used to (re)set the pointer to the uart.
	 *  \param u* pointer to the UART object this controller should use.
	 */

	void setTemp(int temp);
	/*!	\fn void setTemp(int temp)
	 *	\brief sets the temprature of the emulator
	 *	\param temp in celcius degrees
	 *	\return void
	 */

	//uart:
	void setResponseFlag();
	/*!	\fn void setResponseFlag()
	 *	\brief sets the response flag
	 *	\return void
	 */
	 
	void writeResponse(char * response);
	/*!	\fn void writeResponse(char * response)
	 *	\brief writes a response in the response pool 
	 *	\param response a char array with two positions
	 *	\return void
	 */

	void main();
};

#endif