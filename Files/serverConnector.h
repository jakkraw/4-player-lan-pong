#pragma once
#include "activeInterface.h" 
#include "connection.h"
#include "tcpSocket.h"
#include "pointersInterface.h"
#include "idInterface.h"
#include <thread>

typedef std::unordered_map<::id, Connection*> ConnectionMap;

class ConnectionsContainer {
	std::mutex connMapMtx;
	ConnectionMap connMap;
public:
	void add(Connection*c) {
		std::lock_guard<std::mutex> lock(connMapMtx);
		delete connMap[c->getId()];
		connMap[c->getId()] = c;
	}
	void remove(::id id) {
		std::lock_guard<std::mutex> lock(connMapMtx);
		if (Connection* c = connMap[id]) {
			c->stop();
			SAFE_DELETE(c);
			connMap.erase(id);
		}
	}
	Connection* get(::id id) {
		std::lock_guard<std::mutex> lock(connMapMtx);
		return connMap[id];
	}
	void closeAllConnections() {
		std::lock_guard<std::mutex> lock(connMapMtx);
		for (auto c : connMap) {
			c.second->stop();
			SAFE_DELETE(c.second);
		}connMap.clear();
	}

	int size() {
		std::lock_guard<std::mutex> lock(connMapMtx);
		return int(connMap.size());
	}

	void lock() {connMapMtx.lock();}
	void unlock() { connMapMtx.unlock(); }

	Connection* operator [](::id i) { return get(i); }

	ConnectionMap::iterator begin() { return connMap.begin(); }

	ConnectionMap::const_iterator begin() const { return connMap.cbegin(); }

	ConnectionMap::iterator end() { return connMap.end(); }

	ConnectionMap::const_iterator end() const { return connMap.cend(); }
};

class ServerConnector: public ActiveInterface ,protected PointersInterface,public IdInterface{
	ConnectionsContainer* connMapPtr;
	std::thread connectorThread;
	TcpSocket* listenSocket;
public:
	void start(ConnectionsContainer* connMapPtr, ::id id);
	void run();
	void stop();
	std::string ServerConnector::getInfo();
};