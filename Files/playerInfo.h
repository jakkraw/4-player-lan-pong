#pragma once

#include "player.h"
#include "lanPaddleController.h"

class PlayerInfo
{

private:
	Player* player;
	int id;
	LanPaddleController paddle;

public:
	PlayerInfo();

};