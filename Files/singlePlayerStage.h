#ifndef SINGLE_PLAYER_STAGE_H
#define SINGLE_PLAYER_STAGE_H

#include "stageInterface.h"
#include "gameEntityCollection.h"
#include "ball.h"
#include "paddle.h"
#include "paddleController.h"
#include "wall.h"
#include "player.h"
#include "game.h"


class SinglePlayerStage: public StageInterface
{
protected:
	GameEntityCollection ec;
	Player p1, p2,p3,p4;
	PaddleController controller1, controller2;


protected:
	
	bool initialize();
	void render();
	void update();
	void collisions();
	void shutdown();

};

#endif