#include <iostream>
#include "UART.h"
//#include "Motor.h"
#include "Protocol.h"
#include <stdint.h>

int main(){
	
	try{
		UART u = UART("/dev/ttyAMA0", 9600);
		/*Motor m = Motor(u);
		m.turn(0, 1600);*/

		while (true){
			u.executeCommand(DOOR_LOCK_REQ, LOCK_CMD);
			u.executeCommand(DOOR_LOCK_REQ, UNLOCK_CMD);
		}
	}
	catch (std::exception &e){
		std::cerr << e.what();
	}

	
	
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}