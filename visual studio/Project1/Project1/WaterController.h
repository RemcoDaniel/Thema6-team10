/**
 * \class The WaterController controller class
 *
 * Makes sure the water is at the right level
 *
 */


#ifndef _WATERCONTROLLER_H
#define _WATERCONTROLLER_H

#include "pRTOS.h"
#include "WaterSensor.h"
#include "WashingMachineController.h"
#include "Pump.h"
#include "Valve.h"
#include "UART.h"

class WashingMachineController;
class UART;

class WaterController : public RTOS::task {
private:
	char * command = 0;
	WaterSensor watersensor;
	Pump pump;
	Valve valve;
	UART *uartptr;
	WashingMachineController * wascontroller;
	RTOS::flag response_flag, start_flag;
	RTOS::pool< int > water_level_pool;
	RTOS::pool< char > response_pool;
	RTOS::mutex water_level_mutex, response_mutex;
	RTOS::clock interval_clock;

	int getNewWaterLevel();
	int getWaterLevel();
	void pumping(bool on);		// on = 1 , off = 0
	void valving(bool on);		// on = 1 , off = 0

	//uart:
	char readResponse();
	char uartTask(char request, char command);

public:
	WaterController(WashingMachineController * wascontroller);
	
	/*! \fn void setUartPointer(UART *u)
	 *  \brief Used to (re)set the pointer to the uart.
	 *  \param u* pointer to the UART object this controller should use.
	 */
	void setUartPointer(UART * u);
	 
	/*!	\fn void setWaterLevel(int level)
	 *	\brief sets the waterlevel
	 *	\param level the waterlevel in precent
	 *	\return void
	 */
	void setWaterLevel(int level);

	/*!	\fn void startWaterController()
	 *	\brief sets start_flag
	 *	\return void
	 */
	void startWaterController();

	//uart:
	/*!	\fn void setResponseFlag()
	 *	\brief sets the response flag
	 *	\return void
	 */
	void setResponseFlag();
	
	/*!	\fn void writeResponse(char response)
	 *	\brief writes a response in the response pool 
	 *	\param response a char
	 *	\return void
	 */
	void writeResponse(char response);

	void main();
};

#endif