/**
 * \class Motor boundry class
 *
 * Used as an interface to get the appropiate bytes for the uart
 *
 */

#ifndef _MOTOR_H
#define _MOTOR_H

class Motor {
public:
	Motor();
	
	char * getMotorSpeedCommand();
	/*! \fn char * getMotorSpeedCommand()
	 *	\brief Returns the bytes to get the speed of the motor
	 *	\return a char pointer to a 2 char array
	 */
	
	char * turn(bool dir, int speed);
	/*! \fn char * turn(bool dir, int speed)
	 *	\brief Sets the right speed and the right direction (or left direction) of the motor
	 *	\param dir the direction of the motor. false = right, true = left
	 *	\param speed the speed of the rpm. 0 <= x <= 1600
	 *	\return the bytes used to set the RPM in a direction.
	 */
};

#endif