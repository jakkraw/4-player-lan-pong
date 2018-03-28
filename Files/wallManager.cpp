#include "wallManager.h"

WallManager::WallManager() :defaultWalls(8) {
	Vector screenSize = { 600,600 };
	double thickness = 300;
	double offset = 2;

	defaultWalls[Wall::DOWN].setSize({ screenSize.x, thickness });
	defaultWalls[Wall::DOWN].setPosition({ 0, screenSize.y - offset });
	defaultWalls[Wall::DOWN].type = Wall::DOWN;


	defaultWalls[Wall::UP].setSize({ screenSize.x, thickness });
	defaultWalls[Wall::UP].setPosition({ 0,-thickness + offset });
	defaultWalls[Wall::UP].type = Wall::UP;

	defaultWalls[Wall::RIGHT].setSize({ thickness, screenSize.y });
	defaultWalls[Wall::RIGHT].setPosition({ screenSize.x - offset,0 });
	defaultWalls[Wall::RIGHT].type = Wall::RIGHT;

	defaultWalls[Wall::LEFT].setSize({ thickness, screenSize.y });
	defaultWalls[Wall::LEFT].setPosition({ -thickness + offset,0 });
	defaultWalls[Wall::LEFT].type = Wall::LEFT;


	double scale = 1.3;
	Vector sc = { scale,scale };

	defaultWalls[Wall::LEFT_UPPER].setScale(sc);
	defaultWalls[Wall::LEFT_UPPER].setPosition(defaultWalls[Wall::LEFT_UPPER].getTexture()->getRotCenter()*-1);
	defaultWalls[Wall::LEFT_UPPER].setAngle(45);
	defaultWalls[Wall::LEFT_UPPER].type = Wall::LEFT_UPPER;

	defaultWalls[Wall::LEFT_LOWER].setScale(sc);
	defaultWalls[Wall::LEFT_LOWER].setPosition({ defaultWalls[Wall::LEFT_LOWER].getTexture()->getRotCenter().x*-1,defaultWalls[Wall::LEFT_LOWER].getTexture()->getRotCenter().x*-1 + screenSize.y });
	defaultWalls[Wall::LEFT_LOWER].setAngle(-45);
	defaultWalls[Wall::LEFT_LOWER].type = Wall::LEFT_LOWER;

	defaultWalls[Wall::RIGHT_UPPER].setScale(sc);
	defaultWalls[Wall::RIGHT_UPPER].setPosition({ defaultWalls[Wall::RIGHT_UPPER].getTexture()->getRotCenter().x*-1 + screenSize.x,defaultWalls[Wall::RIGHT_UPPER].getTexture()->getRotCenter().x*-1 });
	defaultWalls[Wall::RIGHT_UPPER].setAngle(315 - 180);
	defaultWalls[Wall::RIGHT_UPPER].type = Wall::RIGHT_UPPER;

	defaultWalls[Wall::RIGHT_LOWER].setScale(sc);
	defaultWalls[Wall::RIGHT_LOWER].setPosition(defaultWalls[Wall::RIGHT_LOWER].getTexture()->getRotCenter()*-1 + screenSize);
	defaultWalls[Wall::RIGHT_LOWER].setAngle(45 - 180);
	defaultWalls[Wall::RIGHT_LOWER].type = Wall::RIGHT_LOWER;

}


Wall * WallManager::createWall(Wall::Type pe) {
	return new Wall(defaultWalls[pe]);
}

void WallManager::add(Wall* wall)
{
	walls[wall->type] = wall->getId();
}



void WallManager::remove(Wall::Type type)
{
	walls.erase(type);
}

void WallManager::remove(::id id)
{
	for (auto p : walls) {
		if (id == p.second) {
			walls.erase(p.first);
			return;
		}
	}

}

::id WallManager::get(Wall::Type type)
{
	return walls[type];
}
