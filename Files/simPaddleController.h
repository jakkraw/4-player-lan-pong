#pragma once

#include "paddleController.h"
#include "idInterface.h"
#include "msg.h"

class SimPaddleController : public PaddleController,public IdInterface {

	bool up, down, left, right;
public:

	void update() {
		if (Paddle * entity = (Paddle*)getEntity()) {
			if (left) { entity->moveLeft(); }
			if (right) { entity->moveRight(); }
			if (up) { entity->rotateRight(); }
			if (down) { entity->rotateLeft(); }
		}
	}

	void set(ControllerInfoMsg*msg) {
		this->up = msg->up;
		this->down = msg->down;
		this->left = msg->left;
		this->right = msg->right;
	}
};