#include "server.h"
#include "console.h"

void Server::push(MsgInterface * msg){
	msg->sender = this->getId();
	if (!msg->to) {
		for (auto &c : connMap)
			if (c.second&& c.second->isActive()) {c.second->push(msg->copy());}
		SAFE_DELETE(msg);
	}
	else if (Connection* c = connMap[msg->to]) {
				if (c->isActive()) {c->push(msg);}
	}
	else {
		SAFE_DELETE(msg);
	}
}

void Server::push(std::unordered_map<::id, UdpMsgInterface*>& map)
{
	for (auto &msg : map) {
		msg.second->sender = this->getId();
	}
	bool sent = false;
	for (auto &c : connMap)
		if (c.second&& c.second->isActive()) {
			std::unordered_map<::id, UdpMsgInterface*> m;
			for (auto &x : map) {m[x.first] = x.second->copy();}
			c.second->push(m); 
			sent = true;
		}

	for (auto& t : map) {
		delete t.second;
	}

}


void Server::push(UdpMsgInterface * msg){
	msg->sender = this->getId();
	connMap.lock();
	if (!msg->to) {
		for (auto &c : connMap)
			if (c.second&& c.second->isActive()) {	c.second->push(msg->copy());}
		SAFE_DELETE(msg);
	}
	else if (Connection* c = connMap[msg->to]) {
		if (c->isActive()) { c->push(msg); }
	}
	else {
		SAFE_DELETE(msg);
	}
	connMap.unlock();
}

void Server::poll(MsgQueue& queue){
	connMap.lock();
	for (auto &c : connMap)
		if (c.second && c.second->isActive()) {c.second->poll(queue);}
	connMap.unlock();
}

void Server::poll(UdpMsgMap& map){
	connMap.lock();
	for (auto &c : connMap)
		if (c.second && c.second->isActive()) {c.second->poll(map);}
	connMap.unlock();
}

void Server::disconnect(::id id){
	connMap.remove(id);
}

bool Server::initialize(){
	setId(generateId());
	serverConn.start(&connMap, getId());
	console()->print(serverConn.getInfo().c_str());
	Log("[S] Started!");
	return true;
}

void Server::shutdown(){
	serverConn.stop();
	connMap.closeAllConnections();
	Log("[S] Stopped.");
}



