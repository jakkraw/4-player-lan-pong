#ifndef SURFACE_MANAGER_H
#define SURFACE_MANAGER_H

#include "constants.h"
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace SurfaceManagerNS
{
	const std::string PATH = "Illuminati.png";
	const std::string ROOT = "./Assets/";



}

class SurfaceManager
{
public:
	SDL_Surface * surf;
	std::unordered_map<std::string,SDL_Surface*> sufraces;
	SurfaceManager();
	void loadTextures();
	void shutdown();


};




#endif








