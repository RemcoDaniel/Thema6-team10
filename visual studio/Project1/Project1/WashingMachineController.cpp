#include "WashingMachineController.h"

WashingMachineController::WashingMachineController() :
	task{ 0, "beep" },
	interval_clock{ this, 500 * bmptk::us, "washing_timer" },
	temp_reached_flag{ this, "temp_reached" },
	level_reached_flag{ this, "water_reached" }
{}


void WashingMachineController::setTempReached() {
	temp_reached_flag.set();
}

void WashingMachineController::registerObserver() {

}

void WashingMachineController::removeObserver() {

}

void WashingMachineController::notifyObserver() {

}

//void startWasprogramma(Wasprogramma wp);

void WashingMachineController::stopWasprogramma() {

}

void WashingMachineController::start() {

}

void WashingMachineController::stop() {

}

void WashingMachineController::errorHandling() {

}


void WashingMachineController::main() {
	/*lsp::init();
	for (bool stop = false; !stop; ) {
		wait(start_flag);
		for (;;) {
			lsp::set(1);
			wait(interval_clock);
			lsp::set(0);
			auto event = wait(stop_flag + interval_clock);
			if (event == stop_flag) { break; }
		}
	}*/
}