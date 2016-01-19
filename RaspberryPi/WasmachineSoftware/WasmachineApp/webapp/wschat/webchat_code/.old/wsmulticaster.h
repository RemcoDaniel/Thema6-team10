
// Jan Zuurbier september 2014
// De broadcaster is thread-safe gemaakt.
// Dit is nodig omdat verschillende threads hier
// mogelijk tegelijkertijd gebruik van maken.


#ifndef BROADCASTER_H
#define BROADCASTER_H

#include "websocket.h"
#include <list>
#include <mutex>

class Broadcaster
{
public:
	Broadcaster();
	~Broadcaster();
	void add(WebSocket* ws);
	void remove(WebSocket* ws);
	void broadcast(const string& message);

private:
	list<WebSocket*> theList;
	std::mutex   mutex;  // Mutex for list locking
};

#endif // BROADCASTER_H
