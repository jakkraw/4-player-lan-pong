#pragma once
#include "wall.h"

class WallManager {
	std::unordered_map<Wall::Type, ::id> walls;
	std::vector<Wall> defaultWalls;
public:
	WallManager();
	Wall* createWall(Wall::Type pe);
	void add(Wall* wall);
	void remove(Wall::Type type);
	void remove(::id id);
	::id get(Wall::Type type);
};