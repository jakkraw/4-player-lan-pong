#pragma once

#include <thread>
#include "tcpSocket.h"
#include"connection.h"
#include "config.h"
#include "macro.h"
#include "msg.h"
#include "netComponentInterface.h"
#include "clientServerMsg.h"

class Client :public NetComponentInterface
{
	std::thread connectThread;
	Connection* connection;
public:
	bool initialize();
	bool connect(std::string ip, int port = 0);
	void shutdown();
	void push(MsgInterface* msg);
	void push(UdpMsgInterface*msg);
	void push(std::unordered_map<::id, UdpMsgInterface*>& map);
	void poll(std::queue<MsgInterface*>& queue);
	void poll(std::unordered_map<::id, UdpMsgInterface*>&map);
};