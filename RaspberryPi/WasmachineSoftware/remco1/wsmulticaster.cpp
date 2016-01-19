// Jan Zuurbier september 2014
// De broadcaster is thread-safe gemaakt.
// Dit is nodig omdat verschillende threads hier
// mogelijk tegelijkertijd gebruik van maken.

#include "wsmulticaster.h"

Broadcaster::Broadcaster()
{
}

Broadcaster::~Broadcaster()
{
}

void Broadcaster::add(WebSocket* ws){
	std::lock_guard<std::mutex> lock(mutex);
	theList.push_back(ws);
}

void Broadcaster::remove(WebSocket* ws){
	std::lock_guard<std::mutex> lock(mutex);
	theList.remove(ws);
}

void Broadcaster::broadcast(const string& message){
	std::lock_guard<std::mutex> lock(mutex);
	for (std::list<WebSocket*>::iterator it=theList.begin(); it!=theList.end(); ++it)
		(*it)->sendTextMessage(message);
}

