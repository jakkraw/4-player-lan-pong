#include "surfaceManager.h"

SurfaceManager::SurfaceManager()
{
	surf = NULL;
}

void SurfaceManager::loadTextures()
{


	
	surf = IMG_Load((SurfaceManagerNS::ROOT+SurfaceManagerNS::PATH).c_str());
	if (surf == NULL) { printf("-----------------Unable to load surface %s! SDL_image Error: %s\n",
		SurfaceManagerNS::PATH.c_str(),
		IMG_GetError());
		SDL_FreeSurface(surf);
		delete(surf);
	}

}

void SurfaceManager::shutdown()
{
	SDL_FreeSurface(surf);
}
