#include "paddle.h"

#include "ball.h"

Paddle::Paddle():Entity()
{
	xVelocityStoppingForce = 300;
	xVelocityLimit = 500;
	xVelocitySpeed = 800;
	angularStoppingForce = 200;
	angularLimit = 300;
	angularSpeed = 500;
	backToNormalPositionSpeed = 500;

	Vector size = { 290,74 };
	Vector offset = { 23,33 };
	Vector scale = { 0.5,0.5 };

	getTexture()->setName("paddle.png");
	getTexture()->setSize(size);
	getTexture()->setRotCenter(size / 2);
	getHitbox()->setType(Hitbox::RECTANGLE);
	getHitbox()->setOffset(offset);
	getHitbox()->setSize(size - offset * 2);
	setScale(scale);

}


void Paddle::update()
{
	Entity::update();

	xVelocityLimitFunction();
	angularLimitFunction();

	if (toggleVelocityStoppingForce) {
		xVelocityStoppingForceFunction();
	}

	if (toggleAngularStoppingForce) {
		angularStoppingForceFuction();
		backToNormalPositionFunction();
	}

	toggleVelocityStoppingForce = true;
	toggleAngularStoppingForce = true;

}


void Paddle::handleCollision(Entity * copy, Entity * original)
{
	if (IS_A(original,Ball*)){
		physics()->defaultRectCircleCollision(this, copy);
		setAngularVelocity(0);
		setVelocity({ 0, getVelocity().y });
	}
	else{
		Entity::handleCollision(copy, original);
	}

}

void Paddle::setOrientation(double degree)
{
	this->orientation = degree;
	setAngle(degree);
}

void Paddle::moveLeft(){
	Vector force = Vector(xVelocitySpeed*time()->getFrameTime(), 0);
	force = math::rotatePointByAngle(force, { 0,0 }, orientation);
	setVelocity(getVelocity() - force);
	toggleVelocityStoppingForce = false;
}

void Paddle::moveRight(){
	Vector force = Vector(xVelocitySpeed*time()->getFrameTime(), 0);
	force = math::rotatePointByAngle(force, { 0,0 }, orientation);
	setVelocity(getVelocity() + force);
	toggleVelocityStoppingForce = false;
}

void Paddle::rotateRight(){
	setAngularVelocity(getAngularVelocity() + angularSpeed *time()->getFrameTime());
	toggleAngularStoppingForce = false;
}

void Paddle::rotateLeft(){
	setAngularVelocity(getAngularVelocity() - angularSpeed * time()->getFrameTime());
	toggleAngularStoppingForce = false;
}


void Paddle::xVelocityStoppingForceFunction()
{
	double stoppingForce = xVelocityStoppingForce * time()->getFrameTime();
	Vector velocity = getVelocity();
	if (orientation == 0 || orientation == 180) {
		if (velocity.x > 0.5)setVelocity({velocity.x - stoppingForce, velocity.y});
		if (velocity.x < -0.5)setVelocity({ velocity.x +stoppingForce, velocity.y });
	}
	else if( orientation== 90 || orientation == 270) {
		if (velocity.y > 0.5)setVelocity({ velocity.x, velocity.y - stoppingForce });
		if (velocity.y < -0.5)setVelocity({velocity.x, velocity.y + stoppingForce });
	}

}

void Paddle::angularStoppingForceFuction()
{
	double av = getAngularVelocity();

	if (av > 0) {
		setAngularVelocity(av - angularStoppingForce * time()->getFrameTime());
	}	
	else if (av < 0) {
		setAngularVelocity(av + angularStoppingForce * time()->getFrameTime());
	}
		
	double epsilon = 0.5;
	if (abs(av) < epsilon) setAngularVelocity(0);
}

void Paddle::xVelocityLimitFunction()
{
	Point v = getVelocity();
	if (orientation == 0 || orientation == 180) {
		if (v.x > xVelocityLimit) setVelocity({ xVelocityLimit, v.y });
		if (v.x < -xVelocityLimit) setVelocity({ -xVelocityLimit, v.y });
	}
	else if (orientation == 90 || orientation == 270) {
		if (v.y > xVelocityLimit) setVelocity({v.x, xVelocityLimit });
		if (v.y < -xVelocityLimit) setVelocity({v.x, -xVelocityLimit });
	}

}

void Paddle::angularLimitFunction()
{
	double av = getAngularVelocity();
	if (av > angularLimit) setAngularVelocity(angularLimit);
	if (av < -angularLimit) setAngularVelocity(-angularLimit);
	
}

void Paddle::backToNormalPositionFunction()
{
	double angle = getAngle()-orientation;
	angle = remainder(angle, 360);
	if (angle < 0) {
		angle += 360;
	}

	double eps = 1;

	if (math::isValueBetween(angle, 0+ eps, 90- eps) ||
		math::isValueBetween(angle, 180+ eps, 270- eps)){
		setAngularVelocity(getAngularVelocity() - backToNormalPositionSpeed*time()->getFrameTime());
	}
	else if (math::isValueBetween(angle, 90+ eps, 180- eps) ||
		math::isValueBetween(angle, 270+ eps, 360- eps)){
		setAngularVelocity(getAngularVelocity() + backToNormalPositionSpeed*time()->getFrameTime());
	}
		
}


