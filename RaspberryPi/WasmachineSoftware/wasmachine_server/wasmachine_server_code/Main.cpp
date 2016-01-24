#include "TCPServer.h"
#include "WasmachineApp.h"
#include "wsmulticaster.h"
#include <memory>
#include <thread>
#include <chrono>

int main(int argc, char **argv)
{
	Broadcaster *b = new Broadcaster();
	WasmachineApp *wasmachine = new WasmachineApp(b);
	std::shared_ptr<TCPServer> wasServer(new TCPServer);
	std::thread serverthread(&TCPServer::runTCPServer, wasServer,wasmachine, 8080);
	serverthread.join();

	return 0;
}