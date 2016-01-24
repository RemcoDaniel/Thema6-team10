#ifndef WASMACHINEAPP_H
#define WASMACHINEAPP_H

#include "websocket.h"
#include "wsmulticaster.h"
#include <string>
#include <queue> 
#include "WasProgramma.h"

class WasmachineApp : public WebSocketListener{
public:
	WasmachineApp(Broadcaster *broadcaster);
	void onTextMessage(const string & msg, WebSocket *ws) override;
	void onClose(WebSocket *ws) override;
	void broadcastMessage(const string & msg);
	Broadcaster* getBroadcaster();

	bool isMsg();
	wasprogrammaStruct getLastMsg();

	wasprogrammaStruct jsonStringtoWasprogramma(const string &msg);

private:
	wasprogrammaStruct wasprogramma;
	std::queue<wasprogrammaStruct> msgQue;
	Broadcaster *broadcaster;
};

#endif