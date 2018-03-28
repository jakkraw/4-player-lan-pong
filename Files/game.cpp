#include "game.h"

#include "simulation.h"
#include "console.h"
#include "introStage.h"


bool Game::initialize()
{
	simulation = new Simulation();

	GamePointers* ptr = getGlobalGamePointers();
	ptr->console = new Console();
	ptr->game = this;


	if(!Engine::initialize()) return false;	
	if (!console()->initialize()) return false;
	
	setStage(new IntroStage());
	
	return true; 
}

void Game::update(){
	if (console()->getShow())
		console()->update();

	Engine::update();
}

void Game::render(){
	Engine::render();

	if (console()->getShow())
			console()->draw();

	if (getShowFPS()) {
		graphics()->setColor(color::BLACK & color::ALPHA25);
		graphics()->drawRect(0, 0, 100, 60);
		graphics()->getText()->changeFontSize(18);
		graphics()->getText()->changeFontColor(color::WHITE);
		graphics()->drawText(std::to_string(time()->getFPS()), 20, 20);
	}
}

void Game::specialCases()
{
	if ((input()->keyDown(SDLK_LALT) || input()->keyDown(SDLK_RALT)) && input()->keyPressed(SDLK_RETURN)) {
		input()->clearKey(SDLK_RETURN);
		graphics()->setFullscreen(!graphics()->getFullscreen());
	}

	if (input()->keyPressed(SDLK_TAB) || input()->keyPressed(SDLK_BACKQUOTE)) {
		input()->clearKey(SDLK_TAB);
		input()->clearKey(SDLK_BACKQUOTE);
		console()->setShow(!console()->getShow());
	}

	if (input()->keyPressed(SDLK_ESCAPE)) {
			exit();
	}

}

void Game::handleEvents(SDL_Event event)
{
	Engine::handleEvents(event);
	if (console()->getShow())console()->handleEvents(event);
}

void Game::shutdown() {
	simulation->shutdown();
	console()->shutdown();
	deleteGlobalGamePointers();

	Engine::shutdown();
}


bool Game::getShowFPS(){
	return showFPS;
}

void Game::setShowFPS(bool show){
	showFPS = show;
}
