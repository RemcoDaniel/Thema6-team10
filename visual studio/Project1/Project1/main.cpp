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

int main(){
	try{
		std::cout << "wasprogramma maken\n";
		// eerst wasprogramma maken! deze kan hij dan uit gaan voeren		// dit moet nog anders worden gedaan!!!!!!
		Wasprogramma wasprog = Wasprogramma(40, 20, 500 MS, 1);
		std::cout << "making washingmachine\n";
		// de wasmachinecontroller maakt ook alle andere controllers aan + de boundary objecten + de uart
		WashingMachineController wasctrl = WashingMachineController(wasprog);
		//std::cout << "starting washingmachine\n";
		//wasctrl.main();
		std::cout << "starting RTOS\n";
		RTOS::run();
		std::cout << "verkeerd gegaan in de main.cpp\n";
	}
	catch (std::exception &e){
		std::cerr << e.what();
	}
		
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}