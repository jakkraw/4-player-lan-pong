#pragma once

//class PaddleController;
#include "player.h"
//#include <string>

class PlayerManager
{
public:
	enum Position { DOWN, UP, LEFT, RIGHT };
private:
	Player players[4];
	int tab[40];
	Position position;
	PaddleController* controller;
	Paddle* paddle;
	bool active;
	std::string name;
public:
	Player();
	bool isActive();
	std::string getName();
	Position getPosition();
	Paddle* getPaddle();
	PaddleController* getController();
	void setPosition(Position pos);

	void update();

	void setPaddle(Paddle* paddle);
	void setName(std::string name);
	void setActive(bool active);
	void setController(PaddleController* controller);

};