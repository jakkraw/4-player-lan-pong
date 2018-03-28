#include"udpSocket.h"

int UdpSocket::recv(char* buffer, int bufferLength)
{
	int structLength = sizeof(pAddr);
	int	result = recvfrom(socket, buffer, bufferLength, 0, (sockaddr *)(&pAddr), &structLength);
		if (result == SOCKET_ERROR){
			switch (int error = WSAGetLastError()){
			case WSAEINTR:
			case WSAENOTSOCK:
				Log("[UdpSock]WSAENOTSOCK");
				return result;
			case WSAETIMEDOUT:
				Log("[UdpSock]WSAETIMEDOUT");
				return result;
			default:
				LogError("[UdpSock]recv Error %d\n", error);
				return result;
			}
		}
	return result;
}

int UdpSocket::send(char* data, int dataLength)
{
	int result = sendto(socket, data, dataLength, 0, (sockaddr*)(&pAddr), sizeof(pAddr));
	if (result == SOCKET_ERROR)
	{
		switch (int error = WSAGetLastError()){
		case WSAENOTSOCK:
			Log("[UdpSock]WSAENOTSOCK");
			return result;
		default:
			LogError("[UdpSock]send Error %d\n", error);
			return result;
		}
	}
	return result;
}

//bool UdpSocket::setReciveTimeout(int miliseconds)
//{
//	std::string timeout = std::to_string(miliseconds).c_str();
//	if (setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, timeout.c_str(), sizeof(timeout)) < 0)
//		return false;
//	return true;
//}
