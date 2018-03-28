#pragma once

#include"socket.h"

struct TcpSocket : public Socket
{
	TcpSocket() :Socket(SOCK_STREAM) {};
	TcpSocket* accept();
	int connect();
	int listen(int maxConnections);
	int send(char*buffer, int bufferLength);
	int recv(char*buffer, int bufferLength);

};