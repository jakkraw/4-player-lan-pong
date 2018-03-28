#pragma once
#include "msg.h"
#include <queue>
#include <unordered_map>

typedef std::queue<MsgInterface*> MsgQueue;
typedef std::unordered_map<::id, UdpMsgInterface*> UdpMsgMap;

class NetInterface{
public:
	virtual void push(MsgInterface* msg)=0;
	virtual void push(std::unordered_map<::id, UdpMsgInterface*>&map) = 0;
	virtual void push(UdpMsgInterface*msg)=0;
	virtual	void poll(MsgQueue& queue)=0;
	virtual void poll(UdpMsgMap&map)=0;
};