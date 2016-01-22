// file main.cpp

#include <iostream>
#include "UART.h"
#include "Protocol.h"
#include <stdint.h>
#include "Motor.h"
#include "MotorController.h"
#include "Heater.h"
#include "TempSensor.h"
#include "TempController.h"
#include "Pump.h"
#include "Valve.h"
#include "WaterSensor.h"
#include "WaterController.h"
#include "Door.h"
#include "SoapDispenser.h"
#include "WebController.h"

int main(){
	try{
		// de wasmachinecontroller maakt ook alle andere controllers aan + de boundary objecten + de uart
		WasmachineApp app = WasmachineApp();
		WashingMachineController wasctrl = WashingMachineController(app);
		RTOS::run();
	}
	catch (std::exception &e){
		std::cerr << e.what();
	}
		
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}