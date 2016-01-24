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
	
	/*!	\fn		void onTextMessage(const string & msg, WebSocket *ws) override
	 *	\brief	When a textmessage is recieved, this is called.
	 *	\param	msg string
	 *	\param	ws pointer to the websocket
	 */
	void onTextMessage(const string & msg, WebSocket *ws) override;
	
	/*!	\fn		void sendTextMessage(const string &msg, WebSocket *ws)
	 *	\brief	When a textmessage is recieved, this is called.
	 *	\param	msg string
	 *	\param	ws pointer to the websocket
	 */
	void sendTextMessage(const string &msg, WebSocket *ws);
	
	/*!	\fn		void onClose(WebSocket *ws)
	 *	\brief	Sends the msg to the websocket
	 *	\param	msg string
	 *	\param	ws pointer to the websocket
	 */
	void onClose(WebSocket *ws) override;
	 
	/*!	\fn		void broadcastMessage(const string & msg)
	 *	\brief	When the websocket is closed, this is called
	 *	\param	ws pointer to the websocket
	 */
	void broadcastMessage(const string & msg);
	 
	/*!	\fn		Broadcaster* getBroadcaster()
	 *	\brief	Returns a pointer to the broadcaster
	 *	\param	msg string
	 *	\return Pointer to the broadcaster
	 */
	Broadcaster* getBroadcaster();
	
	/*!	\fn		isMsg()
	 *	\brief	Returns whether the msgQeu is empty.
	 *	\return Returns whether the msgQeu is empty
	 */
	bool isMsg();
	
	/*!	\fn		wasprogrammaStruct getLastMsg()
	 *	\brief	Returns the last message from the struct
	 *	\return Returns the last message from the struct
	 */
	wasprogrammaStruct getLastMsg();
	
	/*!	\fn		wasprogrammaStruct jsonStringtoWasprogramma(const string &msg)
	 *	\brief	Returns the last message from the struct
	 *	\param msg the string in json that needs to be translated
	 *	\return Returns the translated json string
	 */
	wasprogrammaStruct jsonStringtoWasprogramma(const string &msg);

private:
	wasprogrammaStruct wasprogramma;
	std::queue<wasprogrammaStruct> msgQue;
	Broadcaster *broadcaster;
};

#endif