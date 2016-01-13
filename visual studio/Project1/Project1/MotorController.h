// file MotorController.h

#ifndef _MOTORCONTROLLER_H
#define _MOTORCONTROLLER_H

class MotorController {
protected: // of private
	// theUART
public:
	MotorController();
	unsigned int getMotorSpeed();		// protected?
	void setMotorSpeed(unsigned int rpm);	// protected?
	void updateState();
	void setUART();
};

#endif