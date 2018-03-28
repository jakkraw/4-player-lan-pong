#include "playerManager.h"

PlayerManager::PlayerManager(): defaultPosition(4),players(4,nullptr)
{
	double dist = 50;
	Vector ss = { 600,600 };
	defaultPosition[Player::DOWN] = { ss.x / 2.0,ss.y - dist };
	defaultPosition[Player::UP] = { ss.x / 2.0,dist };
	defaultPosition[Player::LEFT] = { dist,ss.y/2.0 };
	defaultPosition[Player::RIGHT] = { ss.x - dist,ss.y/2.0};
}
