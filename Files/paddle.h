#ifndef PADDLE_H
#define PADDLE_H
#include "entity.h"

class Paddle:public Entity{
protected:
	deg orientation;

	bool toggleVelocityStoppingForce;
	bool toggleAngularStoppingForce;

	double xVelocityStoppingForce;
	double xVelocityLimit;
	double xVelocitySpeed;

	double angularStoppingForce;
	double angularLimit;
	double angularSpeed;

	double backToNormalPositionSpeed;

	void xVelocityStoppingForceFunction();
	void angularStoppingForceFuction();
	void xVelocityLimitFunction();
	void angularLimitFunction();
	void backToNormalPositionFunction();

public:
	Paddle();

	void update();
	void handleCollision(Entity * entCopy, Entity * original);
	void setOrientation(double degree);

	void moveLeft();
	void moveRight();
	void rotateRight();
	void rotateLeft();

};

#endif