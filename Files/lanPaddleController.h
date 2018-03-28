#pragma once

#include "paddleController.h"
#include "idInterface.h"

class LanPaddleController : public PaddleController, public IdInterface  {
public:
	bool up, down, left, right;
	LanPaddleController() : PaddleController() {

	};

	void update() {
		if (input()->keyDown(moveLeft)) { left = true; }
		else left = false;
		if (input()->keyDown(moveRight)) { right = true; }
		else right = false;
		if (input()->keyDown(rotateLeft)) { down = true; }
		else down = false;
		if (input()->keyDown(rotateRight)) { up = true; }
		else up = false;

		PaddleController::update();
	}

};