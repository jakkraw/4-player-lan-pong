#include "serverConnector.h"
#include "console.h"

void ServerConnector::start(ConnectionsContainer * connMapPtr, ::id id) {
	if (!isActive()) {
		ActiveInterface::start();
		this->connMapPtr = connMapPtr;
		setId(id);
		connectorThread = std::thread(&ServerConnector::run, this);
		Log("[SC] Started!");
	}
}

void ServerConnector::run() {
	TcpSocket* tcpRecvSocket = nullptr;
	TcpSocket* tcpSendSocket = nullptr;
	UdpSocket* udpSend = nullptr;
	UdpSocket* udpRecv = nullptr;


	listenSocket = new TcpSocket();
	listenSocket->setMyAddr(Config::network.serverListenPort, ANY_IP);
	listenSocket->bind();
	listenSocket->listen(5);
	Log(getInfo());
	

	while (isActive())
	{
		SAFE_DELETE(udpRecv);
		SAFE_DELETE(udpSend);
		SAFE_DELETE(tcpSendSocket);
		SAFE_DELETE(tcpRecvSocket);

		if (!(tcpRecvSocket = listenSocket->accept())) { continue; }

		//recv Msg from player
		char buffer[BUFLEN];
		if (tcpRecvSocket->recv(buffer, BUFLEN) <= 0) { continue; }
		ConnectMsg* cMsg = (ConnectMsg*)deserialize(buffer);


		//assign ports
		static int portsUsed = 0;
		int tcpSendPort = Config::network.serverPortAssignRangeFrom + 3 * portsUsed++;
		int udpRecvPort = tcpSendPort + 1;
		int udpSendPort = tcpSendPort + 2;

		tcpSendSocket = new TcpSocket();
		tcpSendSocket->setMyAddr(tcpSendPort, ANY_IP);
		tcpSendSocket->setPAddr(cMsg->clientTcpRecvPort, ipToString(listenSocket->getPIp()));
		if (tcpSendSocket->connect() < 0) { continue; }



		ResponseMsg rMsg;
		rMsg.sender = getId();
		rMsg.serverUdpRecvPort = udpRecvPort;
		rMsg.hostUdpSendPort = udpSendPort;
		rMsg.clientId = generateId();
		if (tcpSendSocket->send(serialize(&rMsg), rMsg.size) < 0) { continue; }


		udpSend = new UdpSocket();
		udpSend->setMyAddr(rMsg.hostUdpSendPort, ANY_IP);
		udpSend->setPAddr(cMsg->clientUdpRecvPort, ipToString(listenSocket->getPIp()));
		if (udpSend->bind() < 0) { continue; }

		udpRecv = new UdpSocket();
		udpRecv->setMyAddr(rMsg.serverUdpRecvPort, ANY_IP);
		if (udpRecv->bind() < 0) { continue; }

		ConnectionInfo connInfo;
		connInfo.udpRecvSocket = udpRecv;
		connInfo.udpSendSocket = udpSend;
		connInfo.tcpSendSocket = tcpSendSocket;
		connInfo.tcpRecvSocket = tcpRecvSocket;
		connInfo.connectionId = rMsg.clientId;

		Connection*connection = new Connection(connInfo);
		connection->start();
		connMapPtr->add(connection);


		udpRecv = nullptr;
		udpSend = nullptr;
		tcpSendSocket = nullptr;
		tcpRecvSocket = nullptr;

		Log("[S][:%i] Connected ID:%i %s ", connMapPtr->size(), rMsg.clientId, listenSocket->pInfo().c_str());

	}

	SAFE_DELETE(udpRecv);
	SAFE_DELETE(udpSend);
	SAFE_DELETE(tcpSendSocket);
	SAFE_DELETE(tcpRecvSocket);
	Log("[S] Stopped waiting for Players.");


}

void ServerConnector::stop() {
	if (isActive()) {
		ActiveInterface::stop();
		SAFE_DELETE(listenSocket);
		connectorThread.join();
	}
}

std::string ServerConnector::getInfo() {
	while (!listenSocket) { Sleep(10); };
	std::ostringstream stream;
	stream << "[S] PORT:[" << listenSocket->getPort() << "] IP:";
	for (auto x : network()->getMyIp()) { stream << "[" << x << "]"; }
	return stream.str();
}