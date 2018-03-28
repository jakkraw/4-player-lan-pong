#pragma once
#include "entity.h"

class Wall : public Entity
{
public:
	enum Type { DOWN, UP, LEFT, RIGHT, LEFT_UPPER, RIGHT_UPPER, LEFT_LOWER, RIGHT_LOWER , NONE };
	Type type;
	Wall();
	Wall(const Wall& wall);
};

