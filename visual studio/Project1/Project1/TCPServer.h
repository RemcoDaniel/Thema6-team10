#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "WasmachineApp.h"
#include <exception>
#include "websocket.h"
#include <iostream>
#include <string>


class TCPServer{
public:

	/*!	\fn
	 *	\brief Blocking neverending function that listens to packages on the set port
	 *	\param *wasapp A pointer to the washmachineApp
	 *	\param port Which port to use
	 */
	void runTCPServer(WasmachineApp *wasapp, int port);
};

#endif