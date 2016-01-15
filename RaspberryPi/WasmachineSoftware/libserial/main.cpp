#include <iostream>
#include "UART.h"
//#include "Motor.h"
#include "Protocol.h"
#include <stdint.h>

int main(){
	std::cout << "HALLOOOOOOO\n";
	try{
		UART u = UART("/dev/ttyAMA0", 9600);
		/*Motor m = Motor(u);
		m.turn(0, 1600);*/


		char y[3] = {MACHINE_REQ, START_CMD,'\0'};
		u.executeCommand(y);

		char x[3] = {SIGNAL_LED_REQ, ON_CMD, '\0'};
		u.executeCommand(x);
	}
	catch (std::exception &e){
		std::cerr << e.what();
	}

	
	
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}
