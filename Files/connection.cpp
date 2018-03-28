#include"connection.h"

Connection::Connection(ConnectionInfo info) {
	
	tcpSendSocket = info.tcpSendSocket;
	tcpRecvSocket = info.tcpRecvSocket;
	udpRecvSocket = info.udpRecvSocket;
	udpSendSocket = info.udpSendSocket;
	setId(info.connectionId);
}

Connection::~Connection() {
	SAFE_DELETE(udpSendSocket);
	SAFE_DELETE(udpRecvSocket);
	SAFE_DELETE(tcpRecvSocket);
	SAFE_DELETE(tcpSendSocket);
}

void Connection::start()
{
	ActiveInterface::start();

	udpRecv = std::thread(&Connection::udpRecvThread, this);
	udpSend = std::thread(&Connection::udpSendThread, this);
	tcpRecv = std::thread(&Connection::tcpRecvThread, this);
	tcpSend = std::thread(&Connection::tcpSendThread, this);
	
}

void Connection::stop()
{
	ActiveInterface::stop();

	udpSendSocket->shutdown();
	udpRecvSocket->shutdown();
	tcpRecvSocket->shutdown();
	tcpSendSocket->shutdown();

	udpSendSocket->close();
	udpRecvSocket->close();
	tcpRecvSocket->close();
	tcpSendSocket->close();

	msgRecvQueue.close();
	msgSendQueue.close();
	udpSendMap.close();
	udpRecvMap.close();
	
	tcpRecv.join();
	udpRecv.join();
	tcpSend.join();
	udpSend.join();

}


void Connection::udpSendThread()
{
	UdpMsgMap msgMap;

	while (isActive()){
		udpSendMap.condPassTo(msgMap);
		for (auto &m : msgMap){
			if (udpSendSocket->send(serialize(m.second), m.second->size) <= 0) {goto end;}
		}
		for (auto &m : msgMap) {
			delete m.second;
		}msgMap.clear();
	}
end:
	for (auto &m : msgMap) {
		SAFE_DELETE(m.second);
	}msgMap.clear();
	Log("[NT]UdpSendThread Closed.");
	
}

void Connection::tcpSendThread()
{
	MsgQueue msgQueue;
	while (isActive())
	{
		msgSendQueue.condPassTo(msgQueue);
		while (!msgQueue.empty()) {
			MsgInterface* msg = msgQueue.front();
			if (tcpSendSocket->send(serialize(msg), msg->size) <= 0) {goto end;}
			SAFE_DELETE(msg);
			msgQueue.pop();
		}
	}

end:
	while (!msgQueue.empty()) {
		delete	msgQueue.front();
		msgQueue.pop();
	}
	Log("[NT]TcpSendThread Closed.");

	
}

void Connection::udpRecvThread()
{
	char buffer[MAX_BUFF_LEN];
	int dataLength = MAX_BUFF_LEN;

	while (isActive()){
		memset(buffer, 0, MAX_BUFF_LEN);
		if (udpRecvSocket->recv(buffer, dataLength) <= 0) {break;}
		UdpMsgInterface* msg = (UdpMsgInterface*)deserialize(buffer);
		udpRecvMap.push({ msg->getId(),msg });
	}
	
	Log("[NT]UdpRecvThread Closed.");

}

void Connection::tcpRecvThread()
{
	char buffer[MAX_BUFF_LEN];
	int dataLength = MAX_BUFF_LEN;
	void* msg = nullptr;
	int bytesAssigned;
	int msgSize;
	int offset;
	bool  recvMsg = false;
	while (isActive()){
		offset = 0;	
		int recvBytes = tcpRecvSocket->recv(buffer, dataLength);
		if (recvBytes <= 0) { goto error; }

		while (offset < recvBytes) {
			if (!recvMsg) {
				bytesAssigned = 0;
				msgSize = *(int*)(buffer+offset);
				msg = malloc(msgSize);
				recvMsg = true;
			}

			if (bytesAssigned + recvBytes-offset < msgSize) {
				memcpy((void*)(int(msg) + bytesAssigned), buffer + offset, recvBytes-offset);
				bytesAssigned += recvBytes-offset;
				offset += bytesAssigned;
			}else {
				memcpy((void*)(int(msg) + bytesAssigned), buffer + offset, msgSize - bytesAssigned);
				msgRecvQueue.push((MsgInterface*)msg);
				recvMsg = false;
				offset += msgSize - bytesAssigned;
			}

		}	
	}
	error:
	Log("[NT]TcpRecvThread Closed.");
}


void Connection::poll(std::queue<MsgInterface*>&queue) {
	msgRecvQueue.passTo(queue);
}

void Connection::poll(std::unordered_map<::id, UdpMsgInterface*>& map) {
	udpRecvMap.passTo(map);
}

void Connection::push(std::unordered_map<::id, UdpMsgInterface*>& map) {
	udpSendMap.getFrom(map);
}

void Connection::push(MsgInterface * msg) {
	msgSendQueue.push(msg);
}

void Connection::push(UdpMsgInterface * msg) {
	udpSendMap.push({ msg->getId(),msg });
}
