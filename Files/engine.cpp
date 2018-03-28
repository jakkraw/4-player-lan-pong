#include "engine.h"

bool Engine::initialize()
{
	Pointers* ptr = getGlobalPointers();
	ptr->engine = this;
	ptr->graphics = new Graphics();
	ptr->input = new Input();
	ptr->time = new Time();
	ptr->audio = new Audio();
	ptr->physics = new Physics();
	ptr->network = new Network();
	stage = nullptr;


	Config::load();
	if (!input()->initialize()) return false;
	if (!graphics()->initialize()) return false;
	if (!audio()->initialize()) return false;
	if (!time()->initialize()) return false;
	if (!physics()->initialize()) return false;
	if (!network()->initialize()) return false;
	Log("[Engine] Fully initialized.");
	return true;
}

void Engine::handleEvents(SDL_Event event)
{
		switch (event.type)
		{
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						Log("[Engine] Window Closed.");
							exit();
						break;
					case SDL_WINDOWEVENT_RESIZED:
						Log("Window %d resized to %dx%d",
							event.window.windowID, event.window.data1,
							event.window.data2);
						if (!graphics()->getFullscreen()) {
							Config::graphics.windowWidth = event.window.data1;
							Config::graphics.windowHeight = event.window.data2;
						}
						
						break;
					case SDL_WINDOWEVENT_MOVED:
						Log("Window %d moved %dx%d",
							event.window.windowID, event.window.data1,
							event.window.data2);
						if (!graphics()->getFullscreen()) {
							Config::graphics.windowX = event.window.data1;
							Config::graphics.windowY = event.window.data2;
						}

						break;
					default: break;
				}
				break;
			default:{
				input()->handleEvents(event);
			}
		}
}

void Engine::exit(){
	quit = true;
}

void Engine::shutdown()
{
	setStage(nullptr);
	audio()->shutdown();
	graphics()->shutdown();
	input()->shutdown();
	time()->shutdown();
	network()->shutdown();
	physics()->shutdown();
	Config::save();
	deleteGlobalPointers();
	SDL_Quit();
	Log("[Engine] Engine Successfully Closed.");
}

void Engine::update() { if (stage) { stage->update(); } }
void Engine::ai() { if (stage) { stage->ai(); } }
void Engine::collisions() { if (stage) { stage->collisions(); } }
void Engine::render() { if (stage) { stage->render(); } };

void Engine::run()
{
	if (!initialize()){
		LogCritical("[Engine] Initialize Failed!");
		exit();
	}
	
	while (!quit){
		time()->manageFPS();

		SDL_Event	event;
		while (SDL_PollEvent(&event))	{
			handleEvents(event);
		}

		specialCases();

		if (!paused){
			update();
			ai();
			collisions();
		}
		
		graphics()->startDraw();
		render();
		graphics()->endDraw();

		input()->clearAll();
	}

	shutdown();
	
}

bool Engine::getPause(){
	return paused;
}

void Engine::setPause(bool pause)
{
	this->paused = pause;
	if(paused)audio()->pauseMusic();
	else audio()->resumeMusic();
}

void Engine::setStage(StageInterface * stage)
{
	if (this->stage) {
		this->stage->shutdown();
		SAFE_DELETE(this->stage);
	}
	if (stage) {
		this->stage = stage;
		this->stage->initialize();
	}	
}



