#pragma once

class Game;
class Console;

struct GamePointers {
	Game* game = nullptr;
	Console* console = nullptr;
};

class GPointersInterface
{
private:
	friend class Game;
	GamePointers* getGlobalGamePointers();
	static GamePointers globalGamePointers;
	GamePointers* localGamePointers;
	void deleteGlobalGamePointers();
public:
	GPointersInterface();
	~GPointersInterface();
	void setLocalGamePointers(GamePointers pointers);
	inline Game* game() const {return localGamePointers->game;}
	inline Console* console() const {return localGamePointers->console;}

};
