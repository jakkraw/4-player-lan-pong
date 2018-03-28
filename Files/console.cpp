#include "console.h"

#include "singlePlayerStage.h"
#include "introStage.h"
#include "lanStage.h"
#include "server.h"
#include "client.h"


bool Console::getShow() const{
	return show;
}

void Console::setShow(bool show)
{
	this->show = show;
	if (show){SDL_StartTextInput();}
	else {SDL_StopTextInput();}
}

bool Console::initialize() {
	assignCommands();
	setShow(false);

	for (auto &p : cmdMap) {
		print(p.first);
	}

	return true;
}

void Console::shutdown()
{
	for (auto x : history)
		delete(x.second);
}


void Console::draw()
{

	Vector ss = graphics()->getScreenSize();

	Vector size = ss * 3.0 / 4.0;

	Point consolePos((ss.x - size.x) / 2, (ss.y - size.y) / 2);
	graphics()->setColor(color::BLACK & color::ALPHA25);
	graphics()->drawRect(Rect(consolePos,size));

	Vector inputSize(size.x - 20, 23);
	Point inputPos(consolePos.x + 10,  consolePos.y + size.y - 50);
	graphics()->setColor(color::GREEN & color::ALPHA50);
	graphics()->drawRect(Rect(inputPos,inputSize));

	Point textPos(inputPos.x + 10, inputPos.y);
	graphics()->getText()->changeFontSize(18);
	graphics()->getText()->changeFontColor(color::WHITE);
	graphics()->drawText(inputText, int(textPos.x), int(textPos.y));


	int distance = (graphics()->getText()->getFontSize() + 10);
	int i = 0;
	for (auto const &item : history)
	{
		double pos_y = textPos.y - ++i*distance;
		item.second->setPosition(Point(textPos.x, pos_y));
		if (pos_y > consolePos.y) { 
			graphics()->drawTexture(item.second);
		}
	}

}

void Console::update()
{
	
	if (input()->keyPressed(SDLK_c) && input()->keyDown(SDLK_LCTRL)){
		input()->clearKey(SDLK_c);
		SDL_SetClipboardText(inputText.c_str());
	}

	if (input()->keyPressed(SDLK_x) && input()->keyDown(SDLK_LCTRL)){
		input()->clearKey(SDLK_x);
		SDL_SetClipboardText(inputText.c_str());
		inputText.clear();
	}

	if (input()->keyPressed(SDLK_v) && input()->keyDown(SDLK_LCTRL)){
		input()->clearKey(SDLK_v);
		inputText = SDL_GetClipboardText();
	}

	if (input()->keyPressed(SDLK_UP)){
		input()->clearKey(SDLK_UP);

		if (history.size() > historyIndex){
			++historyIndex;
			inputText = std::next(history.begin(), historyIndex - 1)->first;
		}
	}


	if (input()->keyPressed(SDLK_DOWN)){
		input()->clearKey(SDLK_DOWN);

		if (historyIndex >= 1){
			--historyIndex;
			if (historyIndex != 0){
				inputText = std::next(history.begin(), historyIndex - 1)->first;
			}
			else{
				inputText.clear();
			}	
		}
	}		

	input()->clearAll(false);

}

void Console::assignCommands()
{

	cmdMap["fps"] = [&](std::vector<std::string> tokens){
		game()->setShowFPS(!game()->getShowFPS());
	};

	

	cmdMap["join"] = [&](std::vector<std::string> tokens) {
		Client* c = network()->getClient();
		c->initialize();
		
		if (tokens.size() == 1)
			if (!c->connect(LOCALHOST)) { goto error; }			
		if (tokens.size() == 2)
			if (!c->connect(tokens[1])) { goto error; }		
		if (tokens.size() == 3)
			if (!c->connect(tokens[1], std::stoi(tokens[2]))) {goto error;}
		
		game()->setStage(new LanStage());
		return;
	error:
		c->shutdown();
			print("Connection failed.");
		};


	cmdMap["!join"] = [&](std::vector<std::string> tokens) {
		engine()->setStage(new IntroStage());
		network()->getClient()->shutdown();
	};

	cmdMap["host"] = [&](std::vector<std::string> tokens) {
		network()->getServer()->initialize();
		game()->getSimulation()->initialize();
		print("Server started!");
	};

	cmdMap["!host"] = [&](std::vector<std::string> tokens) {
		game()->getSimulation()->shutdown();
		network()->getServer()->shutdown();
		print("Server stopped.");
	};
	
	cmdMap["res"] = [&](std::vector<std::string> tokens) {
		if (tokens.size() == 3)
			try { graphics()->setWindowSize(std::stoi(tokens[1]), std::stoi(tokens[2]));}
			catch (std::invalid_argument e) {LogError("[Console] Bad value.");}	
	};

	cmdMap["lim"] = [&](std::vector<std::string> tokens) {
		static bool e = true;

		time()->setLimitFPS(e = !e);
		
	};

	cmdMap["disp"] = [&](std::vector<std::string> tokens) {
		if (tokens.size() == 2){
			if (tokens[0] == "disp") {
				try {if (graphics()->setDisplay(std::stoi(tokens[1]))){
						print("Display changed succesfully");
					}
				}
				catch (std::invalid_argument e) {
					print("Failed to change display.");
					LogError("[Console] Bad value.");
				}
			}
		}
	};


	cmdMap["song"] = [&](std::vector<std::string> tokens) {
		if (tokens.size() == 2){
			if (tokens.size() < 2) return;
			if (!audio()->playSong(tokens[1])) return;
			print("Song changed to " + tokens[1]);
		}
	};

	cmdMap["volume"] = [&](std::vector<std::string> tokens) {
		if (tokens.size() == 2){
			try {audio()->setVolume(std::stoi(tokens[1]));}
			catch (std::invalid_argument e) {
				LogError("[Console] Bad volume value.");
			}
			print("Volume from 0 to 128 current: " + std::to_string(audio()->getVolume()));
		}
	};

	cmdMap["intro"] = [&](std::vector<std::string> tokens) {
		engine()->setStage(new IntroStage());
		setShow(false);
	};

}

void Console::handleEvents(SDL_Event e)
{

	if (e.type == SDL_TEXTINPUT)
	{
		if (e.text.text[0] == SDLK_BACKQUOTE) return;
		inputText += e.text.text;
		input()->clearKey(e.text.text[0]);
		historyIndex = 0;
	}
		

	while (history.size() > 20)
	{
		delete(history.back().second);
		history.pop_back();
	}

	if (input()->keyPressed(SDLK_BACKSPACE) && inputText.length() > 0) {
		input()->clearKey(SDLK_BACKSPACE);
		inputText.pop_back();
		historyIndex = 0;
	}

	if (input()->keyPressed(SDLK_RETURN)) {
		input()->clearKey(SDLK_RETURN);
		cmd(inputText);
		historyIndex = 0;
		inputText.clear();
	}

}

void Console::print(std::string msg){
	std::lock_guard<std::mutex> locker(mtx);
	graphics()->getText()->changeFontSize(18);
	graphics()->getText()->changeFontColor(color::WHITE);
	history.push_front({msg,graphics()->getText()->textToTexture(msg, graphics()->getRenderer())});

}

void Console::cmd(std::string cmd)
{
	cmd = functions::toLower(cmd);
	std::vector<std::string> tokens = functions::explode(cmd,' ');

	if (tokens.size())
	{
		std::string name = tokens[0];	
		auto found = cmdMap.find(name);
		if (found != cmdMap.end()) {
			print(inputText);
			found->second(tokens);
		}
		else {
			std::stringstream concat;
			for (size_t i = 0; i < tokens.size(); i++) concat << tokens[i]<<" ";
			network()->getClient()->push(new ChatMsg(concat.str()));
		}
	}
}