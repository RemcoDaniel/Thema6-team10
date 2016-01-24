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
#include "WebController.h"
#include "TCPServer.h"
#include "WasmachineApp.h"
#include "wsmulticaster.h"
#include <memory>
#include <thread>
#include <chrono>

int main(){
	try{
		Broadcaster *b = new Broadcaster();
		WasmachineApp *wasmachine = new WasmachineApp(b);
		std::shared_ptr<TCPServer> wasServer(new TCPServer);
		std::thread serverthread(&TCPServer::runTCPServer, wasServer, wasmachine, 8080);
		serverthread.detach();

		WashingMachineController wasctrl = WashingMachineController(wasmachine);
		RTOS::run();
	}
	catch (std::exception &e){
		std::cerr << e.what();
	}
		
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}