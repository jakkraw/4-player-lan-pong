#include "network.h"
#include "server.h"
#include "client.h"


bool Network::initialize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		LogError("Failed. Error Code : %d", WSAGetLastError());
		return false;
	}
	client = new Client();
	server = new Server();
	return true;
}

void Network::shutdown()
{
	client->shutdown();
	server->shutdown();

	SAFE_DELETE(client);
	SAFE_DELETE(server);

	WSACleanup();
}

std::vector<std::string> Network::getMyIp()
{
	std::vector<std::string> ips;
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		return ips;
	}
	hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		return ips;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(in_addr));
		ips.push_back(inet_ntoa(addr));
	}

	return ips;
}


