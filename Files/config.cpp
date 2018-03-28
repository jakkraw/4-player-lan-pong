#include "config.h"

const std::string Config::confName = "config.txt";
struct Config::Graphics Config::graphics;
struct Config::Network Config::network;
struct Config::Time Config::time;
struct Config::Audio Config::audio;
struct Config::Player Config::player;

void Config::remove() {
	::remove(confName.c_str());
}

void Config::load(){
	std::ifstream confFile = std::ifstream(confName);
	std::string str;
	while (std::getline(confFile, str))
	{
		std::string buf;
		std::stringstream ss(str);
		std::vector<std::string> tokens;
		while (ss >> buf) tokens.push_back(buf);

		if (tokens.size() != 2) continue;
		
		graphics.load(tokens);
		network.load(tokens);
		time.load(tokens);
		audio.load(tokens);
		player.load(tokens);
	}
	confFile.close();
}

void Config::save(){
	std::ofstream outFile(confName);

	graphics.save(outFile);
	network.save(outFile);
	time.save(outFile);
	audio.save(outFile);
	player.save(outFile);

	outFile.close();
}

