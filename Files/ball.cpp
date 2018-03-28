#include "ball.h"
#include "paddle.h"


Ball::Ball():Entity()
{

	minBallSpeed = 250;
	maxBallSpeed = 500;
	speedStopForce = 24;

	Vector size = { 50,50 };

	AnimatedTexture texture;
	texture.setName("animatedBall.png");
	texture.setSize(size);
	texture.setRotCenter(size / 2);
	texture.setSourceFrameSize(100, 100);
	texture.setFrames(1, 18);
	texture.setColumns(19);
	texture.setFrameDelay(0.04);
	texture.setCurrentFrame(3);
	setTexture(texture);

	getHitbox()->setType(Hitbox::CIRCLE);
	getHitbox()->setSize(size);

	lastTouched1 = 0;
	lastTouched2 = 0;
}

void Ball::update(){
	Entity::update();
	limitSpeedFunction();
	stopForceFuntion();	
}

void Ball::handleCollision(Entity * copy, Entity * original)
{


	if(IS_A(original,Paddle*)){
		physics()->defaultRectCircleCollision(copy, this);
		setVelocity(getVelocity().getUnitVector()* maxBallSpeed);

		if (original->getId() != lastTouched1 || lastTouched1 == 0) {
			this->lastTouched1 = original->getId();
		}else  if (original->getId() != lastTouched2 || lastTouched2 == 0) {
			this->lastTouched2 = original->getId();
		}

	}else if (IS_A(original, Ball*)){
		physics()->defaultCircleCircleCollision(this, copy);
	}
	else{	
		Entity::handleCollision(copy, original);
	}
		
}


void Ball::limitSpeedFunction()
{
	if (getVelocity().length() > maxBallSpeed) {
		setVelocity(getVelocity().getUnitVector() * maxBallSpeed);
	}

	if (getVelocity().length() < minBallSpeed) {
		setVelocity(getVelocity().getUnitVector() * minBallSpeed);
	}

}

void Ball::stopForceFuntion(){
	setVelocity(getVelocity() - getVelocity() * (speedStopForce / 100)* time()->getFrameTime());
}


