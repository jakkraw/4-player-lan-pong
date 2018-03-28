#pragma once
#include <vector>
#include "logger.h"
#include "pluginInterface.h"

class Server;
class Client;
class Network : public PluginInterface
{
	Server* server;
	Client* client;
public:

	bool initialize();
	void shutdown();
	std::vector<std::string> getMyIp();
	Client* getClient() { return client; }
	Server* getServer() { return server; }
	
};
