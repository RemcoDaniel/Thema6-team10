#include <iostream>
#include "UART.h"
#include "Motor.h"

int main(){

	try{
		UART u = UART("/dev/", 9600);
		Motor m = Motor(u);
		m.turn(0, 1600);
	}
	catch (std::exception &e){
		std::cerr << e.what();
	}
	
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}