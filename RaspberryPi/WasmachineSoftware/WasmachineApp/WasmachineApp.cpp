#include "WasmachineApp.h"

WasmachineApp::WasmachineApp(Broadcaster *b):
broadcaster{ b }{
}

void WasmachineApp::onTextMessage(const string & msg, WebSocket * ws){
	if (msg.compare("FFx02")){
		ws->sendTextMessage("De was wordt gestart");
	}

	//broadcaster->broadcast(msg);
	cout << "Recieved: " << msg << endl;
}

void WasmachineApp::onClose(WebSocket * ws){
	broadcaster->remove(ws);
	delete ws;
}

void WasmachineApp::broadcastMessage(const string & msg){
	broadcaster->broadcast(msg);
}
Broadcaster* WasmachineApp::getBroadcaster(){
	return broadcaster;
}