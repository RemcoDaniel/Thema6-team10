//Gemaakt door Jan Zuurbier, sept 2014
//Vrij om te gebruiken en aan te passen.
//Alleen voor onderwijsdoelen.


#include "websocket.h"
#include <iostream>
#include <openssl/sha.h>
#include "base64.h"

#define CRLF "\r\n"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// WebSocketException
///////////////////////////////////////////////////////////////////////////////

WebSocketException::WebSocketException(const string &message) throw() :
  runtime_error(message) {
}

WebSocketException::WebSocketException(const string &message, const string &detail) throw() :
  runtime_error(message + ": " + detail) {
}

///////////////////////////////////////////////////////////////////////////////
// getline
// deze beschouwt  LF en ook CR+LF en ook CR  als einde-regel markering
// dit is nodig om requests die afkomstig zijn van diverse OS aan te kunnen.
///////////////////////////////////////////////////////////////////////////////
istream & getline(istream & in, string & out) {
	char c;
	out.clear();

	while(in.get(c).good()) {
		if(c == '\r') {
			c = in.peek();
			if(in.good() && c == '\n' ) {
				in.ignore();
			}
			break;
		}
		if(c == '\n') break;
		out.append(1,c);
	}
	return in;
}

///////////////////////////////////////////////////////////////////////////////
// WebSocket
///////////////////////////////////////////////////////////////////////////////


WebSocket::WebSocket(TCPSocket* sock) : data(NULL), datalen(0), closed(false), closing(false)
{
	this->sock = sock;
	thr = new thread(&WebSocket::handleConnection, this);
	thr->detach();
}

WebSocket::~WebSocket()
{
	delete sock;
	if (datalen != 0) delete [] data;
	delete thr;
}


//start de closing-handshake
//stuur een closeframe naar andere kant
//deze zal antwoorden met ook een closeframe
//als die wordt ontvangen stopt de thread en wordt de tcpsocket gesloten
//zie rfc 6455
void WebSocket::close(){
	closing = true;
	try{
		sendClose("1000", 4);
		//1000 is code voor 'normal closure'
	}
	catch(SocketException e){
		closed = true;
	}
}

void WebSocket::setListener(WebSocketListener* l){
	theListener = l;
}

void WebSocket::sendTextMessage(const string &message) throw (WebSocketException, SocketException){
	if(closing) throw WebSocketException("Can't send message, socket is closing");
	size_t payLoadLenght = message.length();
	int frameSize = 2;
	
	char * frame = new char[frameSize + payLoadLenght];
	frame[0] = 0x81;
	if(payLoadLenght > 125){
		frame[1] = 126;
		frame[2] = (uint8_t)((payLoadLenght >> 8) & 0xff);
		frame[3] = (uint8_t)((payLoadLenght) & 0xff);
		frameSize = 4;
	}
	else{
		frame[1] = payLoadLenght;
	}

	//send frame
	memcpy(frame+frameSize, message.c_str(), payLoadLenght);
	sock->send(frame, payLoadLenght+frameSize);

	//throw away frame
	delete[] frame;
}


//stuur een closeframe
//payload dient te bestaan uit een code en een reason
void WebSocket::sendClose(const char* payload, size_t payloadlen) throw (SocketException){
	if(closing) throw WebSocketException("sending close while the websocket is already in closing state");
	if(payloadlen > 125) payloadlen = 125;
	char* frame = new char[2 + payloadlen];
	frame[0] = 0x88; //fin = 1, opc = 0x8
	frame[1] = payloadlen;
	memcpy(frame+2, payload, payloadlen);
	sock->send(frame, payloadlen+2);
	delete[] frame;
}

//stuur een pongframe
void WebSocket::sendPong(const char* payload, size_t payloadlen) throw (SocketException){
	if(closing) return;
	if(payloadlen > 125) payloadlen = 125;
	char* frame = new char [2 + payloadlen];
	frame[0] = 0x8A; //fin = 1, opc = 0xA
	frame[1] = payloadlen;
	memcpy(frame+2, payload, payloadlen);
	sock->send(frame, payloadlen+2);
	delete[] frame;
}

void WebSocket::performHandshake() throw (WebSocketException, SocketException ){
	iostream& stream = sock->getStream();
	string regel;
	string websocketversion;
	string websocketkey;
	bool bad = false;
	if(getline(stream, regel)){
		if(regel.compare(0,3,"GET") != 0){
			bad = true;
		}
	}
	else
		throw WebSocketException("connection closed");
	while(getline(stream, regel)) {
		if(regel.empty())
			break;
		if(regel.compare(0,21,"Sec-WebSocket-Version") == 0)
			websocketversion = regel.substr(23);
		if(regel.compare(0,17,"Sec-WebSocket-Key") == 0)
			websocketkey = regel.substr(19);
	}
	if(bad){
		stream << "HTTP/1.1 400 Bad Request" << CRLF << CRLF;
		throw WebSocketException("Bad request");
	}
	else if(websocketversion.empty() ) {
		stream << "HTTP/1.1 400 Bad Request" << CRLF << CRLF;
		throw WebSocketException("Missing headerfield Sec-WebSocket-Version");
	}
	else if(websocketversion != "13") {
		stream << "HTTP/1.1 400 Bad Request" << CRLF;
		stream << "Sec-WebSocket-Version: 13" << CRLF << CRLF;
		throw WebSocketException("Sec-WebSocket-Version is not 13");
	}
	else if(websocketkey.empty() ) {
		stream << "HTTP/1.1 400 Bad Request" << CRLF << CRLF;
		throw WebSocketException("Missing headerfield Sec-WebSocket-Key");
	}
	else {
		stream << "HTTP/1.1 101 Switching Protocols" << CRLF;
		stream << "Upgrade: websocket" << CRLF;
		stream << "Connection: upgrade" << CRLF;
		websocketkey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		unsigned char hash[SHA_DIGEST_LENGTH];
		memset(hash, 0, SHA_DIGEST_LENGTH);
		SHA1(reinterpret_cast<const unsigned char*>(websocketkey.c_str()), websocketkey.length(), hash);
		string accept_b64 = base64_encode(hash, SHA_DIGEST_LENGTH);
		stream << "Sec-WebSocket-Accept: " << accept_b64 << CRLF << CRLF;
		stream.flush();
	}
}

//lees een frame
//als het geen final frame is plak de payload aan de buffer
//zo kan een bericht worden samengesteld uit meerdere frames.
//als het wel een final frame is, verwerk de het frame op een wijze die
//afhankelijk is van de opcode.

void WebSocket::processFrame() throw(WebSocketException, SocketException){
	cout << "processing frame" << endl;
	unsigned char tmp[4];
	size_t n = sock->recvFully(tmp,2);
	if(n < 2) throw WebSocketException("connection closed while processing frame");
	bool fin  = tmp[0] & 0x80; // bit 0
	int  opc  = tmp[0] & 0x0F; // bits 4, 5, 6, 7
	bool masked = tmp[1] & 0x80; //bit 8
	uint64_t payloadlen  = tmp[1] & 0x7F; // bits 9-15
	if(payloadlen == 126){
		n = sock->recvFully(tmp,2);
		if(n < 2) throw WebSocketException("connection closed while processing frame");
		payloadlen = (tmp[0] << 8) + tmp[1];
	}
	else if(payloadlen == 127){
		n = sock->recvFully(tmp,8);
		if(n < 8) throw WebSocketException("connection closed while processing frame");
		payloadlen = 0;
		for(int i = 0; i < 8; i++){
			payloadlen <<= 8;
			payloadlen += tmp[i];
		}
	}
	char mask[4];
	if (masked){
		n = sock->recvFully(mask,4);
		if(n < 4) throw WebSocketException("connection closed while processing frame");
	}
	if (payloadlen > 0) {
		char * payload = new char[payloadlen];
		n = sock->recvFully(payload, payloadlen);
		if(n < payloadlen) throw WebSocketException("connection closed while processing frame");
		if (masked) {
			char * ptr = payload;
			for (size_t i = 0; i < payloadlen; ++i)
			{
				int j = i % 4;
				ptr[i] = ptr[i] xor mask[j];
			}
		}


		//append payload to databuffer
		char *tmp_data = new char[datalen + payloadlen];
		memcpy(tmp_data, data, datalen);
		memcpy(tmp_data + datalen, payload, payloadlen);
		if(datalen != 0) delete[] data;
		data = tmp_data;
		datalen += payloadlen;
		delete[] payload;
	}

	//if not final frame continu with next frame.
	if(!fin) return;

	//handle message
	switch(opc){
		case 0x1: {
			cout << "textframe" << endl;
			string s (data, datalen);
			if (theListener != NULL) theListener->onTextMessage(s, this);
			break;
		}
		case 0x2: {
			cout << "binary frame" << endl;
			sendClose("1003 binary frames not supported", 32);
			break;
		}
		case 0x8: {
			cout << "closeframe" << endl;
			if(!closing) sendClose(data, datalen);
			closed = true;
			break;
		}
		case 0x9: {
			cout << "ping" << endl;
			sendPong(data, datalen);
			break;
		}
		case 0xA: {
			cout << "pong" << endl;
			break;
		}
		default: {
			cout << "error: invalid opcode" << endl;
			string s = "1002 invalid opcode " + opc;
			sendClose(s.data(), s.size());
			break;
		}
	}

	//clear databuffer
	if(datalen != 0) delete[] data;
	datalen = 0;

}

void WebSocket::handleConnection() {
	cout << "connection to " << sock->getForeignAddress().getAddress() << ":" <<
		sock->getForeignAddress().getPort() << " opened" << endl;
	try{
		performHandshake();
		while(!closed){
			processFrame();
		}

	}
	catch(WebSocketException& e){
		cout << e.what() << endl;
	}
	catch(SocketException& e){
		cout << e.what() << endl;
	}
	cout << "connection to " << sock->getForeignAddress().getAddress() << ":" <<
		sock->getForeignAddress().getPort() << " closed" << endl;
	if(theListener != NULL) theListener->onClose(this);
}
