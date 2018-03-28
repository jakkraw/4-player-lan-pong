#pragma once

#include "stageInterface.h"
#include "paddle.h"
#include "ball.h"
#include "singlePlayerStage.h"

class IntroStage : public StageInterface
{
protected:

	Texture etiTexture;
	Texture* janTexture;
	Texture* kasiaTexture;
	Texture* kubaTexture;
	Texture* rafalTexture;
	Ball* ball;

	Paddle *paddle1,*paddle2;

	bool initialize();
	void render();
	void update();
	void shutdown();

};
