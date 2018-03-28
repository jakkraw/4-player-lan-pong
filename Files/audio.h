#pragma once

#include "SDL.h"
#include "SDL_mixer.h"
#include "resourceManager.h"
#include "logger.h"
#include "config.h"
#include <string>
#include "pluginInterface.h"

class Audio: public PluginInterface
{
private:
	ResourceManager<Mix_Music*>* songs;
	ResourceManager<Mix_Chunk*>* effects;
public:
	bool initialize();
	void shutdown();
	void playEffect(std::string fileName);
	bool playSong(std::string fileName);
	void stopMusic();
	void pauseMusic();
	void resumeMusic();
	void setVolume(int x);
	int getVolume() const;
};
