#pragma once
#include <list>
#include <time.h>
#include "ball.h"
#include "math.h"
class BallManager {
public:
	std::list<std::pair<::id, Ball*>> balls;
	BallManager() {
		srand((int)time(NULL));
	}

	Ball* createBall() {
		Vector ss = { 600,600 };

		Ball* ball = new Ball();
		ball->setPosition({300, 300});
		int r = rand() % 4;
		if (r == 0) {
			ball->setVelocity({ 200,220 });
		}
		if (r == 1) {
			ball->setVelocity({ -200,220 });
		}
		if (r == 2) {
			ball->setVelocity({ 200,-220 });
		}
		if (r==3){
			ball->setVelocity({ -200,-220 });
		}

		return ball;
	}


	void add(Ball*ball) {
		balls.push_back({ ball->getId(),ball });
	}

	void remove(::id id) {

		for (auto &i:balls) {
			if (i.first == id) {
				balls.remove(i);
				break;
			}
		}
	}

	void clear() {

		balls.clear();
	}

	Ball* get(::id id) {
		for (auto &i : balls) {
			if (i.first == id) {
				return i.second;
			}
		}
	}
	
};