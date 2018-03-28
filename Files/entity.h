#pragma once

#include "hitbox.h"
#include "engine.h"
#include "idInterface.h"
#include "pointersInterface.h"

class Entity: public IdInterface,public PointersInterface {
	Texture* texture;
	Hitbox hitbox;
	Vector scale;
	Vector velocity;
	double angularVelocity;

public:
	Entity();
	Entity(const Entity&);
	Entity& operator=(const Entity&);
	virtual ~Entity();

	void initialize();

	void setScale(Vector);
	void setSize(Vector);

	void setPosition(Point);
	void setVelocity(Vector);
	void setAngularVelocity(double);
	void setAngle(double);

	void setTexture(Texture);
	void setTexture(AnimatedTexture);
	void setHitbox(Hitbox);
	
	Hitbox* getHitbox();
	Texture* getTexture() const;
	Vector getScale() const;
	Point getPosition() const;
	Point getVelocity() const;
	double getAngularVelocity() const;
	double getAngle() const;
	
	
	void collide(Entity*);
	virtual void draw();
	virtual void update();
	virtual void handleCollision(Entity*copy,Entity*original);

};
