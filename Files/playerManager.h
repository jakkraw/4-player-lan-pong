#pragma once
#include "player.h"
#include "math.h"
#include "paddle.h"
#include <unordered_map>
class PlayerManager {
	
	std::vector<Point> defaultPosition;


public:
	std::vector<Player*> players;
	PlayerManager();
	~PlayerManager() {
		for (int i = 0; i < 4; i++) {
			if (players[i]) {
				 players[i]->setActive(false);
			}
		}
	}

	void add(Player* player) {
		for (int i = 0; i < 4; i++) {
			if (!players[i]) {
				players[i] = player;
				players[i]->setActive(true);
				players[i]->setPosition((Player::Position)i);
				reset(players[i]->getPosition());
				return;
			}
		}
	}

	void clear() {

		for (int i = 0; i < 4; i++) {
			if (players[i]) {
				players[i]->setActive(false);
			}
		}

		for (int i = 0; i < 4; i++) {
			if (players[i]) {
				players[i] = nullptr;
			}
		}
	}

	int size() {
		int count = 0;
		for (int i = 0; i < 4; i++) {
			if (players[i]) count++;
		}
		return count;
	}

	void reset(Player::Position pos) {
		if (players[pos]) {
			players[pos]->getPaddle()->setPosition(defaultPosition[pos]- players[pos]->getPaddle()->getTexture()->getRotCenter());
			int or = 0;
			if (pos == Player::DOWN) or = 0;
			if (pos == Player::UP) or = 180;
			if (pos == Player::LEFT) or = 90;
			if (pos == Player::RIGHT) or = 270;
			players[pos]->getPaddle()->setOrientation(or);
		}
	}

	void remove(Player::Position pos) {
		if (players[pos]) {
			players[pos] = nullptr;
		}
	}

	void remove(::id id) {
		for (int i = 0; i < 4; i++) {
			if (players[i] && players[i]->getPaddle()->getId() == id) {
				players[i] = nullptr;
				return;
			}
		}
	}

	void remove(Player*player) {
		for (int i = 0; i < 4; i++) {
			if (players[i] && players[i] == player) {
				players[i] = nullptr;
				return;
			}
		}

	}

	Player* find(::id id) {

		for (int i = 0; i < 4; i++) {
			if (players[i] && players[i]->getId() == id) {
				return players[i];
			}
		}
		return nullptr;
	}

	Player* get(Player::Position pos) {
		return players[pos];
	}

	Player* get(::id id) {
		for (int i = 0; i < 4; i++) {
			if (players[i] && players[i]->getPaddle()->getId() == id) {
				return players[i];
			}
		}
		return nullptr;
	}

	void update() {

		for (int i = 0; i < 4; i++) {
			if (players[i]) {
				players[i]->update();
			}
		}

	}


};