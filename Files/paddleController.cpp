#include "paddleController.h"

PaddleController::PaddleController(){
	moveLeft = SDLK_a;
	moveRight = SDLK_d;
	rotateLeft = SDLK_LEFT;
	rotateRight = SDLK_RIGHT;
}

void PaddleController::update(){
	if (Paddle * entity = (Paddle*)getEntity()) {
		if (input()->keyDown(moveLeft)) { entity->moveLeft(); }
		if (input()->keyDown(moveRight)) { entity->moveRight(); }
		if (input()->keyDown(rotateLeft)) { entity->rotateLeft(); }
		if (input()->keyDown(rotateRight)) { entity->rotateRight(); }
	}
}

void PaddleController::setControls(key moveLeft, key moveRight, key rotateLeft, key rotateRight){
	this->moveLeft= moveLeft;
	this->moveRight= moveRight;
	this->rotateLeft= rotateLeft;
	this->rotateRight= rotateRight;
}
