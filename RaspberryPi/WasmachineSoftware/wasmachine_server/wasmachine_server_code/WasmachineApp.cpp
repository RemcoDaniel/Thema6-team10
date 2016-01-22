#include "WasmachineApp.h"

WasmachineApp::WasmachineApp(Broadcaster *b):
broadcaster{ b }{
}

void WasmachineApp::onTextMessage(const string & msg, WebSocket * ws){
	//std::cout << "Msg: " << msg;
	if (msg.find("NEW_CONNECTION ") == 0){
		std::cout << "New connection from: " << msg << std::endl;
	}
	else if (msg.compare("STATUS_STOP") == 0){
		std::cout << "<---------!!!--------->" << std::endl;
		std::cout << "Stopping wasmachine" << std::endl;
		std::cout << "<---------!!!--------->" << std::endl;
		ws->sendTextMessage("Stopped");
	}
	else if(msg.compare("STATUS_START") == 0){
		std::cout << "<---------!!!--------->" << std::endl;
		std::cout << "Starting wasmachine" << std::endl;
		std::cout << "<---------!!!--------->" << std::endl;
		ws->sendTextMessage("Starting");
	}
	else{
		wasprogramma = jsonStringtoWasprogramma(msg);
	}
}

wasprogrammaStruct WasmachineApp::jsonStringtoWasprogramma(const string &msg){
	std::string parsed;
	wasprogrammaStruct decodedStruct;
	//std::cout << "Msg lenght: " << msg.length() << std::endl;
	for (int i = 0; i < msg.length(); ++i){
		if ((msg[i] >= 'a' && msg[i] <= 'z') || (msg[i] >= 'A' && msg[i] <= 'Z') || msg[i] == ':' || msg[i] == ',' || (msg[i] >= '0' && msg[i] <= '9')){
			parsed += msg[i];
		}
	}
	parsed += ",";
	//std::cout << "The parsed string: " << parsed << std::endl;
	std::string splitComma = ",";
	std::string splitDouble = ":";
	std::string sub1;
	std::string sub2;
	int pos = 0;
	int pos2 = 0;
	while((pos = parsed.find(splitComma)) != std::string::npos) {
		sub1 = parsed.substr(0, pos);
		//std::cout << "Sub1 " << sub1 << std::endl;
		parsed.erase(0, pos + splitComma.length());
		if((pos2 = sub1.find(splitDouble)) != std::string::npos){
			sub2 = sub1.substr(0, pos2);
			//std::cout << "Sub2 " << sub2 << std::endl;
			sub1.erase(0, pos2 + splitDouble.length());
			//std::cout << "Sub3 " << sub1 << std::endl;
			if (sub2.compare("water") == 0){
				decodedStruct.waterlevel = atoi(sub1.c_str());
			}
			else if (sub2.compare("temp") == 0){
				decodedStruct.temperature = atoi(sub1.c_str());
			}
			else if (sub2.compare("type") == 0){
				decodedStruct.job = atoi(sub1.c_str());
			}
			else if (sub2.compare("time") == 0){
				decodedStruct.time = atoi(sub1.c_str());
			}
		}
	}

	std::cout << "\nWasprogramma:\nTijd:\t" << decodedStruct.time << " seconden\n";
	std::cout << "Waterlevel:\t" << decodedStruct.waterlevel << " in liters\n";
	std::cout << "Temperatuur:\t" << decodedStruct.temperature << " graden celsius\n";
	std::cout << "Motorjob:\t" << decodedStruct.job << " programma\n";
	return decodedStruct;
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

bool WasmachineApp::isMsg(){
	return msgQue.empty();
}

wasprogrammaStruct WasmachineApp::getLastMsg(){
	auto queFront = msgQue.front();
	msgQue.pop();
	return queFront;
}