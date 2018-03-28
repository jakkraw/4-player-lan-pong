#ifndef GAME_H
#define GAME_H

#include "engine.h"
#include "console.h"
#include "gPointersInterface.h"

class Simulation;
class GameStageInterface;
	
class Game : public Engine,private GPointersInterface
{
	bool showFPS;
	Simulation* simulation;

	bool initialize();
	void update();
	void render();
	void specialCases();
	void handleEvents(SDL_Event event);
	void shutdown();
	
public:
	Simulation* getSimulation() { return simulation; }
	bool getShowFPS();
	void setShowFPS(bool show);
	
};

#endif