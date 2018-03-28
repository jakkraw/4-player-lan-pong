#pragma once

#include <list>
#include <string>
#include <queue>
#include <mutex>
#include <unordered_map>
#include <sstream>
#include "pointersInterface.h"
#include "gPointersInterface.h"
#include "pluginInterface.h"
#include "simulation.h"
#include "server.h"
#include "client.h"
#include "game.h"


class Console: public PointersInterface,protected GPointersInterface,public PluginInterface
{
	bool show;
	std::unordered_map<std::string, std::function<void(std::vector<std::string> tokens)>> cmdMap;
	std::list < std::pair<std::string, Texture*>> history;
	std::mutex mtx;
	Uint32 historyIndex;
	std::string inputText;
	
	void assignCommands();
public:

	bool initialize();
	void shutdown();
	void draw();
	void update();
	void handleEvents(SDL_Event e);

	bool getShow() const;
	void setShow(bool show);
	void cmd(std::string cmd);
	void print(std::string msg);

};
