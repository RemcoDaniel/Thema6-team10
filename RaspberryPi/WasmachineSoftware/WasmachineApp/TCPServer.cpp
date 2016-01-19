#include "TCPServer.h"

void TCPServer::runTCPServer(WasmachineApp *wasapp, int port){
	try {
		// Make a socket to listen for client connections.
		TCPServerSocket servSock(port);
		std::cout << "server running: " << servSock.getLocalAddress().getAddress() << std::endl;
		for (;;) {
			TCPSocket *sock = servSock.accept();
			WebSocket* ws = new WebSocket(sock);
			ws->setListener(wasapp);
			wasapp->getBroadcaster()->add(ws);
		}
	}
	catch (SocketException &e) {
		std::cerr << e.what() << std::endl;           // Report errors to the console
	}
}