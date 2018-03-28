#pragma once
#include "entityCollection.h"
#include "pointersInterface.h"
#include "playerManager.h"
#include "paddle.h"
#include "wall.h"
#include "player.h"
#include "wallManager.h"
#include "ballManager.h"
class GameEntityCollection : public EntityCollection,public PointersInterface {

	
public:
	WallManager wm;
	PlayerManager pm;
	BallManager bm;
	Wall* get(Wall::Type);
	Player* get(Player::Position);
	Entity* get(id);

	Ball* addBall();
	void add(Wall::Type);
	void add(Entity*);
	void add(Player*);
	void remove(id);
	void remove(Wall::Type);
	void remove(Player*);
	void clear();

	void update();
	void render();
	void texUpdate();

	void createBoard();
};