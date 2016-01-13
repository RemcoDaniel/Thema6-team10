#include <iostream>
#include "UART.h"
#include "Motor.h"

int main(){
	UART u = UART();
	Motor m = Motor(u);
	m.turn(0, 1600);
	getchar(); //Scherm op laten staan, wacht op input om af te sluiten
	return 0;
}