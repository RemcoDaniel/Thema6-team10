/**
 * file      Heater.cpp
 * version   0.1
 * author    Remco Nijkamp / Jordan Ranirez / Kevin  Damen / Jeroen Kok
 * date      19-01-2016
 */

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
		// eerst wasprogramma maken! deze kan hij dan uit gaan voeren		// dit moet nog anders worden gedaan!!!!!!
		// de wasmachinecontroller maakt ook alle andere controllers aan + de boundary objecten + de uart
		WashingMachineController wasctrl = WashingMachineController(wasprog);
	}
	catch (std::exception &e){
		std::cerr << e.what();
	}
		
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}