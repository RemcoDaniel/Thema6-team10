#include "TempController.h"

TempController::TempController() :
	task{ 3, "watercontroller" },
	new_temp_flag(this, "new_temp"),
	temp_pool("temp"),
	temp_mutex("temp")
{}



void TempController::main() {

}

