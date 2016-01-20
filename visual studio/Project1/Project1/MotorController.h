/**
 * \class The MotorController controller class
 *
 * Makes sure the motor is rotating at the right RPM in the right direction (or in the left direction)
 *
 *
 */
 
 
#ifndef _MOTORCONTROLLER_H
#define _MOTORCONTROLLER_H

#include "Motor.h"
#include "WashingMachineController.h"
class WashingMachineController;
#include "pRTOS.h"
#include "UART.h"
#include <stdlib.h>
#include <memory>

class MotorController : public RTOS::task {
private:
	Motor motor;
	UART *uartptr;
	WashingMachineController * wascontroller;
	RTOS::flag response_flag, new_job_flag;
	RTOS::pool< int > motor_job_pool, motor_time_pool;
	RTOS::pool< char * > response_pool;
	RTOS::mutex motor_job_mutex, motor_time_mutex, response_mutex;
	RTOS::timer rotate_timer;
	int taak;

	void stopMotor();
	void rotateRight(int speed);
	void rotateLeft(int speed);
	void normalMotorJob(int time);
	void centrifuge(int time);
	void startMotorJob();

	//uart:
	char* readResponse();
	char* uartTask(char * command);

public:
	MotorController(WashingMachineController * wascontroller);
	
	void setUartPointer(UART *u);
	/*! \fn void setUartPointer(UART *u)
	 *  \brief Used to (re)set the pointer to the uart.
	 *  \param u* pointer to the UART object this controller should use.
	 */

	int getMotorSpeed();
	/*! \fn int getMotorSpeed()
	 *	\brief returns the current speed of the motor
	 *	\return the speed int RPM
	 */
	 
	void setMotorJob(int job, int time);
	/*!	\fn void setMotorJob(int job, int time);
	 *	\brief Sets which job should be started and for how long
	 *	\param int which job should be used
	 *	\param int the time in seconds
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