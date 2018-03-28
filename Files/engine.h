#ifndef ENGINE_H
#define ENGINE_H

#include "input.h"
#include "graphics.h"
#include "time.h"
#include "audio.h"
#include "config.h"
#include "macro.h"
#include "functions.h"
#include "logger.h"
#include "network.h"
#include "physics.h"
#include "stageInterface.h"
#include "pointersInterface.h"
#include "pluginInterface.h"


class Engine:protected PointersInterface, public PluginInterface
{
	bool paused;
	bool quit;
	StageInterface* stage;
public:

	void run();
	void exit();
	bool getPause();
	StageInterface* getStage() { return stage; }
	void setPause(bool pause);
	void setStage(StageInterface* stage);

protected:
	
	virtual void handleEvents(SDL_Event event);
	virtual bool initialize();
	virtual void shutdown();
	virtual void update();
	virtual void ai();
	virtual void collisions();
	virtual void render();
	virtual void specialCases() {};

};

#endif
