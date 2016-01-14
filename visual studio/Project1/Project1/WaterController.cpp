#include "WaterController.h"

WaterController::WaterController() :
	task{ 3, "watercontroller" },
	new_level_flag(this, "new_level"),
	water_level_pool("water_level"),
	water_level_mutex("water_level")
{}

float WaterController::getWaterLevel() {

}

void WaterController::setWaterLevel(float wlvl) {

}

bool WaterController::getValveStatus() {

}

void WaterController::setValveStatus(bool state) {

}

void WaterController::updateState() {

}

void WaterController::main() {

}

