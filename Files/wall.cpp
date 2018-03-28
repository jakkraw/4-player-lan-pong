#include "wall.h"
#include "game.h"

Wall::Wall():Entity()
{
	type = NONE;
	Vector size = { 100,100 };
	getTexture()->setName("wall.jpg");
	getTexture()->setSize(size);
	getTexture()->setRotCenter(size / 2);
	getHitbox()->setType(Hitbox::RECTANGLE);
	getHitbox()->setSize(getTexture()->getSize());
}

Wall::Wall(const Wall & wall):Entity(wall)
{
	this->type = wall.type;
	this->setId(generateId());
}

