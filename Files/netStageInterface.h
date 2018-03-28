#pragma once
#include "netComponentInterface.h"
#include "msg.h"
#include "stageInterface.h"


class NetStageInterface : public StageInterface
{
protected:
	NetComponentInterface* nci;
	virtual void processTcpMsg(MsgInterface*msg) = 0;
	virtual void processUdpMsg(UdpMsgInterface*msg) = 0;
	void setNCI(NetComponentInterface* nci) { this->nci = nci; };

public:
	
	void recv() {
		recvTcp();
		recvUdp();
	}
	void send(MsgInterface*msg) {
		if(nci)nci->push(msg);
		else delete msg;
	}
	void send(UdpMsgInterface*msg) {
		if (nci)nci->push(msg);
		else delete msg;
	}

	void send(std::unordered_map<::id, UdpMsgInterface*>& map){
		if (nci)nci->push(map);
		else {
			for (auto&m : map) {delete m.second;}
			map.clear();
		}
	}

private:
	void recvTcp() {
		std::queue<MsgInterface*> queue;
		if (nci)nci->poll(queue);
		while (!queue.empty()) {
			MsgInterface*msg = queue.front();
			processTcpMsg(msg);
			queue.pop();
			delete msg;
		};
	}
	void recvUdp() {
		std::unordered_map<::id, UdpMsgInterface*>map;
		if (nci)nci->poll(map);
		for (auto &m : map) {
			processUdpMsg(m.second);
			delete  m.second;
		};
	}




};