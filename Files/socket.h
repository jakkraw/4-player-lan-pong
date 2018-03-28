#pragma once

#include "logger.h"
#include <string>
#include<winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define ANY_IP "@"
#define LOCALHOST "127.0.0.1"
#define BUFLEN 512 

inline char* ipToString(ULONG ip) {
	struct in_addr addr;
	addr.s_addr = ip;
	return inet_ntoa(addr);
}

inline ULONG stringToIp(std::string ip) {
	if (ip == "@") return INADDR_ANY;
	return inet_addr(ip.c_str());
}


class Socket
{
protected:
	int type;
	SOCKET socket;
	struct sockaddr_in addr;
	struct sockaddr_in pAddr;
	
	Socket(int type);
	virtual ~Socket();
	virtual int send(char*buffer, int bufferLength) = 0;
	virtual int recv(char*buffer, int bufferLength) = 0;

public:	
	void close();
	void shutdown();
	int bind();

	void setMyAddr(int port, std::string ip, ADDRESS_FAMILY af = AF_INET);
	void setPAddr(int port, std::string ip, ADDRESS_FAMILY af = AF_INET);
	
	ULONG getPIp() const;
	int getPPort() const;
	sockaddr_in getPAddr() const;

	ULONG getIp() const;
	int getPort() const;
	sockaddr_in getAddr() const;

	std::string info();
	std::string pInfo();
};