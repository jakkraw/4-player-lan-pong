#pragma once
#include "controllerInterface.h"
#include "paddle.h"

class PaddleController : public ControllerInterface{
protected:
	key moveLeft;
	key moveRight;
	key rotateLeft;
	key rotateRight;
	
public:
	virtual void update();
	PaddleController();
	void setControls(key moveLeft, key moveRight, key rotateLeft, key rotateRight);
};

