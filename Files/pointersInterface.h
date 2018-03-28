#pragma once
#include "macro.h"
class Audio;
class Graphics;
class Input;
class Time;
class Physics;
class Network;
class StageInterface;
class Text;
class Engine;

struct Pointers {
	Engine* engine=nullptr;
	Audio* audio = nullptr;
	Graphics *graphics = nullptr;
	Input *input = nullptr;
	Time *time = nullptr;
	Physics *physics = nullptr;
	Network *network = nullptr;
};

class PointersInterface {
private:
	static Pointers globalPointers;
	Pointers* localPointers;
	friend class Engine;
	Pointers* getGlobalPointers() { return &globalPointers; }
	void deleteGlobalPointers();
protected:
	PointersInterface();
	~PointersInterface();
	void resetPointers();
	Pointers getLocalPointers() const { return *localPointers; }
	inline Engine* engine() const { return localPointers->engine; }
	inline Audio* audio() const { return localPointers->audio; }
	inline Graphics* graphics() const { return localPointers->graphics; }
	inline Input* input() const { return localPointers->input; }
	inline Time* time() const { return localPointers->time; }
	inline Physics* physics() const { return localPointers->physics; }
	inline Network* network() const { return localPointers->network; }
public:
	void setLocalPointers(Pointers pointers);
	
};

