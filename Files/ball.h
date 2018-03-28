#pragma once

#include "entity.h"


class Ball : public Entity
{
	

	double minBallSpeed;
	double maxBallSpeed;
	double speedStopForce;
protected:

	void limitSpeedFunction();
	void stopForceFuntion();

public:
	::id lastTouched1, lastTouched2;
	Ball();
	virtual void update();
	virtual void handleCollision(Entity * copy, Entity * original);
};
