#include "client.h"

bool Client::initialize()
{
	setId(-1);
	
	return false;
}

bool Client::connect(std::string ip,int port)
{
	TcpSocket* sendSocket = nullptr;
	TcpSocket* recvSocket = nullptr;
	UdpSocket* udpSend = nullptr;
	UdpSocket* udpRecv = nullptr;

	sendSocket = new TcpSocket();
	if (port) {sendSocket->setPAddr(port, ip);}
	else {sendSocket->setPAddr(Config::network.serverListenPort, ip);}
	
	Log("[C] Connecting to %s", sendSocket->pInfo().c_str());
	
	ConnectMsg cMsg;
	cMsg.sender = getId();
	cMsg.clientTcpRecvPort = Config::network.clientTcpRecvPort;
	cMsg.clientUdpRecvPort = Config::network.clientUdpRecvPort;
	cMsg.clientUdpSendPort = Config::network.clientUdpSendPort;

	if (sendSocket->connect() < 0) {goto error;}

	if (sendSocket->send(serialize(&cMsg), cMsg.size) <=0){goto error;	}
	//Log("[C] -> %s", cMsg.toString());

	recvSocket = new TcpSocket();
	recvSocket->setMyAddr(cMsg.clientTcpRecvPort, ANY_IP);
	if (recvSocket->bind() <=0) {goto error;}

	if (recvSocket->listen(1) < 0) { goto error; }
	

	//wait for server to connect
	TcpSocket* tmpSocket=nullptr;
	if (!(tmpSocket = recvSocket->accept())) {LogError("[C] Response failed");goto error;}

	SAFE_DELETE(recvSocket);
	recvSocket = tmpSocket;


	char buffer[MAX_BUFF_LEN];
	if (recvSocket->recv(buffer, MAX_BUFF_LEN) < 0) {goto error;}
	ResponseMsg* rMsg = (ResponseMsg*)deserialize(buffer);
	setId(rMsg->clientId);

	udpSend = new UdpSocket();
	udpSend->setMyAddr(cMsg.clientUdpSendPort, ANY_IP);
	udpSend->setPAddr(rMsg->serverUdpRecvPort, ip);

	if (udpSend->bind() < 0){goto error;}

	udpRecv = new UdpSocket();
	udpRecv->setMyAddr(cMsg.clientUdpRecvPort, ANY_IP);
	if (udpRecv->bind() < 0) {goto error;}


	ConnectionInfo connInfo;
	connInfo.udpRecvSocket = udpRecv;
	connInfo.udpSendSocket = udpSend;
	connInfo.tcpRecvSocket = recvSocket;
	connInfo.tcpSendSocket = sendSocket;
	connInfo.connectionId = rMsg->sender;

	connection = new Connection(connInfo);
	connection->start();
	Log("[C] Connected!");

	return true;

error:
	SAFE_DELETE(udpRecv);
	SAFE_DELETE(udpSend);
	SAFE_DELETE(sendSocket);
	SAFE_DELETE(recvSocket);
	return false;

}

void Client::shutdown(){	

	if (connection) {
		connection->stop();
		SAFE_DELETE(connection);
	}
	Log("[C] Stopped.");
}

void Client::push(MsgInterface * msg){
	msg->sender = this->getId();
	if (connection&&connection->isActive()) {connection->push(msg);}
	else SAFE_DELETE(msg);	
}

void Client::push(UdpMsgInterface * msg){
	msg->sender = this->getId();
	if (connection&&connection->isActive()) {connection->push(msg);}
	else SAFE_DELETE(msg);
}

void Client::poll(std::queue<MsgInterface*>& queue){
	if (connection&&connection->isActive()) {connection->poll(queue);}
}

void Client::poll(std::unordered_map<::id, UdpMsgInterface*>& map){
	if (connection&&connection->isActive()) {connection->poll(map);}
}


void Client::push(std::unordered_map<::id, UdpMsgInterface*>& map)
{
	for (auto &msg : map) {
		msg.second->sender = this->getId();
	}
	
	if (connection&& connection->isActive()) { connection->push(map); }
	else{
		for (auto& t : map) {delete t.second;}
	}
}
