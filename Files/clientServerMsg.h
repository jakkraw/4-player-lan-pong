#pragma once

#include "msgInterface.h"

struct ConnectMsg : public MsgInterface
{
	int clientTcpRecvPort;
	int clientUdpSendPort;
	int clientUdpRecvPort;

	ConnectMsg() : MsgInterface(sizeof(ConnectMsg)) {}
};

struct ResponseMsg : public MsgInterface
{
	::id clientId;
	int hostUdpSendPort;
	int serverUdpRecvPort;

	ResponseMsg() : MsgInterface(sizeof(ResponseMsg)) {}
};

