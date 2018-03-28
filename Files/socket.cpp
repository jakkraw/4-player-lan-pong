#include"socket.h"

Socket::Socket(int type)
{
	this->type = type;

	if ((socket = ::socket(AF_INET, type, 0)) == INVALID_SOCKET) {
		LogError("[Sock]initialize Error %d", WSAGetLastError());
	}
}

Socket::~Socket()
{
	shutdown();
	close();
}


int Socket::bind()
{
	int result = ::bind(socket, (const sockaddr*)&addr, sizeof(addr));
	if (result == SOCKET_ERROR) {
		LogError("[Sock]bind Error %d", WSAGetLastError());
	}
	return 1;
}

void Socket::close() {
	int result = ::closesocket(socket);
	if (result == SOCKET_ERROR) {
		switch (int error = WSAGetLastError()) {
		case WSAENOTCONN:
			Log("[Sock][close]WSAENOTCONN");
			return;
		case WSAENOTSOCK:
			Log("[Sock][close]WSAENOTSOCK");
			return;
		default:
			LogError("[Sock][close] error: %d\n", error);
		}
		
	}
}

void Socket::shutdown(){
	int result =::shutdown(socket, 2);
	if (result == SOCKET_ERROR) {
		switch (int error = WSAGetLastError()) {
		case WSAENOTCONN:
			Log("[Sock][shutdown]WSAENOTCONN");
			return;
		case WSAENOTSOCK:
			Log("[Sock][shutdown]WSAENOTSOCK");
			return;
		default:
			LogError("[Sock]Shutdown error: %d\n", error);
		}
	}

}

void Socket::setMyAddr(int port, std::string ip, ADDRESS_FAMILY af){
	this->addr.sin_family = af;
	this->addr.sin_addr.S_un.S_addr = stringToIp(ip);
	this->addr.sin_port = port;
}

void Socket::setPAddr(int port, std::string ip, ADDRESS_FAMILY af) {
	this->pAddr.sin_family = af;
	this->pAddr.sin_addr.S_un.S_addr = stringToIp(ip);
	this->pAddr.sin_port = port;
}

ULONG Socket::getPIp() const{
	return getPAddr().sin_addr.S_un.S_addr;
}

int Socket::getPPort() const{
	return getPAddr().sin_port;
}

sockaddr_in Socket::getPAddr() const{
	return pAddr;
}

ULONG Socket::getIp() const{
	return  getAddr().sin_addr.S_un.S_addr;
}

int Socket::getPort() const{
	return getAddr().sin_port;
}

sockaddr_in Socket::getAddr() const{
	return addr;
}

std::string  Socket::info()
{
	std::string str = "[IP:";
	str += ipToString(getIp());
	str += " PORT:";
	str += std::to_string(getPort());
	str += "]";
	str += (type == SOCK_STREAM) ? "TCP" : "UDP";
	return str;
}

std::string  Socket::pInfo()
{
	std::string str = "[IP:";
	str += ipToString(getPIp());
	str += " PORT:";
	str += std::to_string(getPPort());
	str += "]";
	str += (type == SOCK_STREAM) ? "TCP" : "UDP";
	return str;
}




