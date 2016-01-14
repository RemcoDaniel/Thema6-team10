#include "WaterController.h"

WaterController::WaterController(WaterSensor watersensor, Pump pump, Valve valve) :
	watersensor(watersensor),
	pump(pump),
	valve(valve),
	task{ 3, "watercontroller" },
	interval_clock{ this, 500 * bmptk::us, "interval" },
	water_level_pool("water_level"),
	water_level_mutex("water_level")
{}

int WaterController::getWaterLevel() {
	water_level_mutex.wait();
	int level = water_level_pool.read();
	water_level_mutex.signal();
	return level;
}

void WaterController::setWaterLevel(int level) {
	water_level_mutex.wait();
	water_level_pool.write(level);
	water_level_mutex.signal();
}

void WaterController::main() {
	for (;;) {
		wait(interval_clock);
		int newlevel = getWaterLevel();
		int level = watersensor.getWaterLevel();
		if (level >= newlevel) {
			pump.off();
			if (level > newlevel) {
				valve.on();			// nog ff kijken naar het gedrag van de valve, want dit klopt niet helemaal...
			}
		}
		else valve.off(); pump.on();
	}
}

