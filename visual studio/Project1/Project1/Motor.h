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
	
	// getal van 0 - 1600 rpm
	char * getMotorSpeedCommand();  ///< Returns the bytes to get the RPM of the motor
	
	/** Returns the bytes used to set the RPM in a direction.
	*	param dir the direction of the motor. 0 = right, 1 = left
	*	param speed the speed of the rpm. 0 <= x <= 1600
	*/
	char * turn(bool dir, int speed);			// direction: 0 = right, 1 = left
};

#endif