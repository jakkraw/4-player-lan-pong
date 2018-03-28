#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "math.h"
#include "logger.h"
#include "pluginInterface.h"
class Entity;

class Physics : public PluginInterface
{
	
private:
	Line lines[4];
	Entity*copyA,*copyB;
	bool collideRectRect(Entity* rectA, Entity* rectB);
	bool collideCircleCircle(Entity* circleA, Entity* circleB);
	bool collideRectCircle(Entity* rectA, Entity* circleB);

	bool unstuckRectRect(Entity* rectA, Entity* rectB);
	bool unstuckCircleCircle(Entity* circleA, Entity* circleB);
	bool unstuckRectCircle(Entity* rectA, Entity* circleB);

public:
	bool initialize();
	void shutdown();
	bool collide(Entity*a, Entity*b);

	bool defaultCircleCircleCollision(Entity* circleA, Entity* circleB);
	bool defaultRectCircleCollision(Entity* rectA, Entity* circleB);
	bool defaultRectRectCollision(Entity* rectA, Entity* rectB);
};


#endif