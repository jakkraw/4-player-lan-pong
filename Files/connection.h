#pragma once

#include <thread>
#include <unordered_map>
#include "udpSocket.h"
#include "tcpSocket.h"
#include "logger.h"
#include "config.h"
#include "macro.h"
#include "msgInterface.h"
#include "udpMsgInterface.h"
#include "activeInterface.h"
#include "mtxQueue.h"
#include "mtxUMap.h"

#include "netComponentInterface.h"

#define MAX_BUFF_LEN 512

struct ConnectionInfo
{
	UdpSocket *udpSendSocket, *udpRecvSocket;
	TcpSocket *tcpSendSocket, *tcpRecvSocket;
	::id connectionId;
};


class Connection : public NetComponentInterface, public ActiveInterface
{
private:
	MtxQueue<MsgInterface*> msgSendQueue;
	MtxQueue<MsgInterface*> msgRecvQueue;
	MtxUMap<::id, UdpMsgInterface*>udpSendMap;
	MtxUMap<::id, UdpMsgInterface*>udpRecvMap;


	UdpSocket *udpSendSocket;
	UdpSocket *udpRecvSocket;
	TcpSocket* tcpRecvSocket;
	TcpSocket* tcpSendSocket;

	std::thread udpSend;
	std::thread udpRecv;
	std::thread tcpRecv;
	std::thread tcpSend;

	void udpSendThread();
	void udpRecvThread();
	void tcpRecvThread();
	void tcpSendThread();

public:
	Connection(ConnectionInfo info);
	~Connection();
	void start();
	void stop();


	void push(MsgInterface* msg);
	void push(UdpMsgInterface*msg);
	void push(std::unordered_map<::id, UdpMsgInterface*>& map);
	void poll(std::queue<MsgInterface*>& queue);
	void poll(std::unordered_map<::id, UdpMsgInterface*>&map);

};
