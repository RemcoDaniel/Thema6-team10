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

		char x[2] = {DOOR_LOCK_REQ, LOCK_CMD};
		while (true){
			x[1] = LOCK_CMD;
			u.executeCommand(x);

			x[1] = UNLOCK_CMD;
			u.executeCommand(x);
		}
	}
	catch (std::exception &e){
		std::cerr << e.what();
	}

	
	
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}