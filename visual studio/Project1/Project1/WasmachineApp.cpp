#include "WasmachineApp.h"

WasmachineApp::WasmachineApp(Broadcaster *b):
broadcaster{ b }{
}

void WasmachineApp::onTextMessage(const string & msg, WebSocket * ws){
	if (msg.compare("STATUS_STOP") == 0){
		std::cout << "Sending stopped to webapp" << std::endl;
		ws->sendTextMessage("Stopped");
		msgQue.push()
	}
	else if(msg.compare("STATUS_START") == 0){
		ws->sendTextMessage("Starting");
	}

	std::cout << "Msg Rec: " << msg << std::endl;
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