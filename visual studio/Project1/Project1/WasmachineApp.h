#ifndef WASMACHINEAPP_H
#define WASMACHINEAPP_H

#include "websocket.h"
#include "wsmulticaster.h"
#include <string>

class WasmachineApp : public WebSocketListener{
public:
	WasmachineApp(Broadcaster *broadcaster);
	void onTextMessage(const string & msg, WebSocket *ws) override;
	void sendTextMessage(const string &msg, WebSocket *ws);
	void onClose(WebSocket *ws) override;
	void broadcastMessage(const string & msg);
	Broadcaster* getBroadcaster();

private:
	Broadcaster *broadcaster;
};

#endif