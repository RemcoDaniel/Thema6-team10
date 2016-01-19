#include <iostream>
#include "PracticalSocket.h"
#include "websocket.h"
#include "wsmulticaster.h"
#include <thread>
#include <chrono>


using namespace std;

class MyListener:public WebSocketListener{
public:

	MyListener(Broadcaster *b){
		broadcaster = b;
	}

	void onTextMessage(const string& s, WebSocket* ws){
		broadcaster->broadcast(s);
		cout << "Recieved: " << s << endl;
	}

	void onClose(WebSocket* ws){
		broadcaster->remove(ws);
		delete ws;
	}
private:
	Broadcaster *broadcaster;
};

void runserver(MyListener &ml)
{
	try {
    // Make a socket to listen for client connections.
    TCPServerSocket servSock(8080);
	cout << "server running: " << servSock.getLocalAddress().getAddress() << endl;
    for (;;) {
		TCPSocket *sock = servSock.accept();
		WebSocket* ws = new WebSocket(sock);
		ws->setListener(&	ml);
		ml.broadcaster->add(ws);

    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;           // Report errors to the console
  }
}

void broadCastStock(){
	cout << "Test" << endl;
	for(;;){
		const string stock = "De huidige prijs: " + std::to_string(rand()%100 + 1);
		b.broadcast(stock);
		std::this_thread::sleep_for (std::chrono::seconds(1));
	}
}

int main(int argc, char **argv)
{	
	Broadcaster *b = new Broadcaster();
	MyListener ml = new MyListener(b);
	thread serverthread(runserver(ml));

	//thread stockBroadcast(broadCastStock);
	serverthread.join();
	//stockBroadcast.join();
}
