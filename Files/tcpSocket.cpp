#include"tcpSocket.h"

TcpSocket* TcpSocket::accept()
{
	int structLength = sizeof(pAddr);
	SOCKET newSocket = ::accept(socket, (sockaddr *)(&pAddr), &structLength);
	if (newSocket == INVALID_SOCKET) {
		switch (int error = WSAGetLastError())
		{
		case WSAEINTR:
			Log("[TCPSock]WSAEINTR");
			break;
		case WSAENOTSOCK:
			Log("[TCPSock]WSAENOTSOCK");
			break;
		case WSANOTINITIALISED:
			Log("[TCPSock]WSANOTINITIALISED");
			break;
		default:
			LogError("[TCPSock]accept Error %d\n", error);
		}
		return nullptr;
	}

	TcpSocket* accepted = new TcpSocket();
	accepted->socket = newSocket;
	return accepted;
}

int TcpSocket::listen(int maxConnections)
{
	int result = ::listen(socket, maxConnections);
	if (result == SOCKET_ERROR) {
		LogError("[TCPSock]listen Error %d\n", WSAGetLastError());
		return result;
	}
	return result;
}

int TcpSocket::connect()
{
	int result = ::connect(socket, (sockaddr*)&pAddr, sizeof(pAddr));
	if (result  == SOCKET_ERROR) {
		switch (int error = WSAGetLastError()) {
		case WSAECONNREFUSED:
			Log("[TCPSock]WSAECONNREFUSED");
			break;
		default:
			LogError("[TCPSock]connect Error: %d\n", WSAGetLastError());
			break;
		}
		return result;
	}
	return result;
}

int TcpSocket::send(char* buffer, int bufferLength)
{
	int result = ::send(socket, buffer, bufferLength, 0);
	if (result == SOCKET_ERROR) {
		switch (int error = WSAGetLastError()) {
		case WSAENOTSOCK:
			Log("[UdpSock]WSAENOTSOCK");
			return result;
		default:
			LogError("[TCPSock]send Error %d\n", error);
			return result;
		}
	}
	return result;
}

int TcpSocket::recv(char* buffer, int bufferLength)
{
	int result = ::recv(socket, buffer, bufferLength, 0);
	if (result == SOCKET_ERROR) {
		switch (int error = WSAGetLastError()) {
		case WSAENOTSOCK:
			Log("[TCPSock]WSAENOTSOCK");	
		case WSAEINTR:
			Log("[TCPSock]WSAEINTR");
		default:
			LogError("[TCPSock]recieve Error: %d\n", error);
		}
		return result;
	}


	return result;
}




