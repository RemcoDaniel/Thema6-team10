#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include "PracticalSocket.h"
#include <thread>


using namespace std;

class WebSocketException : public std::runtime_error {
public:

  WebSocketException(const std::string &message) throw();
  WebSocketException(const std::string &message, const std::string &detail) throw();
};

class WebSocketListener;

/** Websocket that is able to communicate with an another websocket over
 *  the underlying TCPSocket. It can send a message to the other side and listens to incomming messages.
 *  @see RFC6455
 *	@author jan.zuurbier@hu.nl
 */

class WebSocket
{
public:

   /**   Construct a web socket
    *   @param sock underlying TCP socket
    */
	WebSocket(TCPSocket* sock);

	~WebSocket();

	/** Closes the websocket connection
	 */
	void close();

	/** Sets the listener for this websocket that handles incomming messages.
	 */
	void setListener(WebSocketListener*);

	/** Send a text message to the other side
	 *  @param message  the message to be sent
	 *  @exception WebSocketException if the message could not be sent.
	 *  @exception SocketException if there is a problem with the underlying tcp socket
	 */
	void sendTextMessage(const string &message) throw (WebSocketException, SocketException);

	/** Get the IP address of the remote web socket
	*/
	string getForeignAddress() { return sock->getForeignAddress().getAddress(); }

private:
	TCPSocket* sock;
	thread* thr;
	WebSocketListener* theListener = NULL;
	char* data;
	int datalen;
	bool closed;
	bool closing;
	void performHandshake() throw(WebSocketException, SocketException);
	void processFrame() throw(WebSocketException, SocketException);
	void handleConnection() ;
	void sendClose(const char*, size_t) throw (SocketException);
	void sendPong(const char*, size_t) throw (SocketException);

};

class WebSocketListener
{
public:
    /** Will be called when a text message from the other side has been recieved
     * @param message the text message recieved
     * @param ws the websocket that recieved the message
     */

	virtual void onTextMessage(const string& message, WebSocket* ws) = 0;

	/** Will be called when the websocket has been close.
	 * @param ws the websocket that has been closed
	 */
	virtual void onClose(WebSocket* ws) = 0;
};


#endif // WEBSOCKET_H
