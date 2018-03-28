#pragma once

class PaddleController;
class Paddle;
#include <string>
#include "idInterface.h"
class Player :public IdInterface
{
public:
	enum Position { DOWN,UP,LEFT,RIGHT};
private:
	Position position;
	PaddleController* controller;
	Paddle* paddle;
	bool active;
	std::string name;
	
public:
	int points;
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
