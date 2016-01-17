#include <iostream>
#include "UART.h"
//#include "Motor.h"
#include "Protocol.h"
#include <stdint.h>

int main(){
	try{
		UART u = UART("/dev/ttyAMA0", 9600);

		//=========================================================================
		// dit moet eerst!!!:
		char start_machine[3] = { MACHINE_REQ, START_CMD, '\0' };
		u.executeCommand(start_machine);
		//=========================================================================

		char x[3] = {DOOR_LOCK_REQ, LOCK_CMD, '\0'};		// denk eraan om hem met \0 af te sluiten
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