#pragma once

#include"socket.h"

struct UdpSocket : public Socket
{
	UdpSocket() :Socket(SOCK_DGRAM) {};
	int recv(char* buffer, int bufferLength);
	int send(char* buffer, int bufferLength);
	//bool setReciveTimeout(int miliseconds);
};