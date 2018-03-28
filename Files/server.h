#pragma once

#include <unordered_map>
#include <thread>
#include "logger.h"
#include "config.h"
#include "tcpSocket.h"
#include "udpSocket.h"
#include"connection.h"
#include "macro.h"
#include "netComponentInterface.h"
#include "pointersInterface.h"
#include "pluginInterface.h"
#include "clientServerMsg.h"
#include "gPointersInterface.h"

#include "serverConnector.h"

class Server: public NetComponentInterface, protected PointersInterface,public PluginInterface,public GPointersInterface
{
	ServerConnector serverConn;
	ConnectionsContainer connMap;
public:
	void push(MsgInterface* msg);
	void push(std::unordered_map<::id, UdpMsgInterface*>&map);
	void push(UdpMsgInterface*msg);
	void poll(MsgQueue& queue);
	void poll(UdpMsgMap& map);
	void disconnect(::id id);
	bool initialize();
	void shutdown();
};
