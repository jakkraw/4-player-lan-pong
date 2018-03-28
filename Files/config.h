#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define SAVE_NAME( name , os , a) \
    do { (os) << (name) << " " << (a) << std::endl; } while(false)

#define SAVE(a, os) SAVE_NAME( #a,(os), (a))

#define LOADINT(name,tokens) \
	if (tokens[0] == #name) name = std::stoi(tokens[1]);

#define LOADDOUBLE(name,tokens) \
	if (tokens[0] == #name) name = std::stod(tokens[1]);

#define LOADSTRING(name,tokens) \
	if (tokens[0] == #name) name = tokens[1];

#define LOADBOOL(name,tokens) \
	if (tokens[0] == #name) name = ((std::stoi(tokens[1]))!=0);

#include "engine.h"
class Config
{
	struct ConfigModule {
		virtual void load(std::vector<std::string> &tokens) = 0;
		virtual void save(std::ofstream &outFile) = 0;
	};
	
	const static std::string confName;
	static std::ifstream confFile;

public:
	static void load();
	static void save();
	static void remove();

	struct Player : public ConfigModule {
		std::string name = "unknown";
		std::string surfaceName = "";
		double prefferedSizeX = 0.5;
		double prefferedSizeY = 0.5;
		void load(std::vector<std::string> &tokens) {
			LOADDOUBLE(player.prefferedSizeX, tokens);
			LOADDOUBLE(player.prefferedSizeY, tokens);
			LOADSTRING(player.surfaceName, tokens);
			LOADSTRING(player.name, tokens);
		}
		void save(std::ofstream &outFile) {
			SAVE(player.surfaceName, outFile);
			SAVE(player.name, outFile);
			SAVE(player.prefferedSizeX, outFile);
			SAVE(player.prefferedSizeY, outFile);
		}
	}static player;

	struct Audio :public ConfigModule {
		unsigned int volume = 0;

		void load(std::vector<std::string> &tokens) {
			LOADINT(audio.volume, tokens);
		}
		void save(std::ofstream &outFile) {
			SAVE(audio.volume, outFile);
		}
	}static audio;

	struct Graphics :public ConfigModule {

		int rendererType = SDL_RENDERER_ACCELERATED;
		int windowWidth = 800;
		int windowHeight = 600;
		int displayNumber = 0;
		bool fullscreen = 0;
		int windowX = -1;
		int windowY = -1;
		const double BASE_WINDOW_WIDTH = 800.0;
		const double BASE_WINDOW_HEIGHT = 600.0;
	


		void load(std::vector<std::string> &tokens) {
			LOADBOOL(graphics.fullscreen, tokens);
			LOADINT(graphics.rendererType, tokens);
			LOADINT(graphics.displayNumber, tokens);
			LOADINT(graphics.windowWidth, tokens);
			LOADINT(graphics.windowHeight, tokens);
			LOADINT(graphics.windowX, tokens);
			LOADINT(graphics.windowY, tokens);
		}
		void save(std::ofstream &outFile) {
			SAVE(graphics.fullscreen, outFile);
			SAVE(graphics.windowWidth, outFile);
			SAVE(graphics.windowHeight, outFile);
			SAVE(graphics.rendererType, outFile);
			SAVE(graphics.displayNumber, outFile);
			SAVE(graphics.windowX, outFile);
			SAVE(graphics.windowY, outFile);
		}
	}static graphics;


	struct Network :public ConfigModule {

		unsigned int serverListenPort = 10600;
		unsigned int serverPortAssignRangeFrom = 10601;
		unsigned int clientUdpSendPort = 10500;
		unsigned int clientUdpRecvPort = 10501;
		unsigned int clientTcpRecvPort = 10502;
		unsigned int maxPlayersConnected = 3;


		void load(std::vector<std::string> &tokens) {
			LOADINT(network.serverListenPort, tokens);
			LOADINT(network.serverPortAssignRangeFrom, tokens);
			LOADINT(network.clientUdpSendPort, tokens);
			LOADINT(network.clientUdpRecvPort, tokens);
			LOADINT(network.clientTcpRecvPort, tokens);
			LOADINT(network.maxPlayersConnected, tokens);
		}
		void save(std::ofstream &outFile) {
			SAVE(network.serverListenPort, outFile);
			SAVE(network.serverPortAssignRangeFrom, outFile);
			SAVE(network.clientUdpSendPort, outFile);
			SAVE(network.clientUdpRecvPort, outFile);
			SAVE(network.clientTcpRecvPort, outFile);
			SAVE(network.maxPlayersConnected, outFile);
		}
	}static network;


	struct Time :public ConfigModule {

		unsigned int FRAME_RATE = 200;
		unsigned int MIN_FRAME_RATE = 60;
		bool limitFPS = true;

		void load(std::vector<std::string> &tokens) {
			LOADINT(time.FRAME_RATE, tokens);
			LOADINT(time.MIN_FRAME_RATE, tokens);
			LOADBOOL(time.limitFPS, tokens);
		}
		void save(std::ofstream &outFile) {
			SAVE(time.FRAME_RATE, outFile);
			SAVE(time.MIN_FRAME_RATE, outFile);
			SAVE(time.limitFPS, outFile);
		}
	}static time;


};

#endif
