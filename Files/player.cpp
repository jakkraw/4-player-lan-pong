#include "player.h"

#include "paddle.h"
#include "paddleController.h"


Player::Player() {
	name = Config::player.name;
	active = false;
	paddle = new Paddle();
	if (!Config::player.surfaceName.empty()) {
		paddle->getTexture()->setName(Config::player.surfaceName);
	}

	controller = new PaddleController();
	controller->setEntity(paddle);
	points = 0;
}

Player::Position Player::getPosition() {
	return position;
}

void Player::setPosition(Position pos) {
	position = pos;
}

void Player::update() {
	controller->update();
}

void Player::setPaddle(Paddle * paddle) {
	delete this->paddle;
	this->paddle = paddle;
	if(controller)
	controller->setEntity(this->paddle);
}

Paddle * Player::getPaddle() {
	return this->paddle;
}

void Player::setName(std::string name) {
	this->name = name;
}

std::string Player::getName() {
	return name;
}

bool Player::isActive() {
	return active;
}

void Player::setActive(bool active) {
	this->active = active;
}

PaddleController * Player::getController() {
	return controller;
}

void Player::setController(PaddleController * controller) {
	Entity* ent = this->controller->getEntity();
	delete this->controller;
	this->controller = controller;
	controller->setEntity(ent);
}

