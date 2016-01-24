#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "WasmachineApp.h"
#include <exception>
#include "websocket.h"
#include <iostream>
#include <string>


class TCPServer{
public:
	void runTCPServer(WasmachineApp *wasapp, int port);
};

#endif