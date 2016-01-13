// file WaterController.h

#ifndef _WATERCONTROLLER_H
#define _WATERCONTROLLER_H

class WaterController {
protected:	// of private?

public:
	WaterController();

	float getWaterLevel();
	void setWaterLevel(float wlvl);
	void setUART();
	bool getValveStatus();
	void setValveStatus(bool state);
	void updateState();
};

#endif