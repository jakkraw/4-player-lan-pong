#ifndef HITBOX_H
#define HITBOX_H

#include "math.h"

class Hitbox
{
public:
	enum Type { CIRCLE, RECTANGLE };
private:
	
	Type type;
	Point size;
	Vector offset;
	double radius;
	bool active;

public:
	
	Hitbox();
	double getRadius();
	Vector getSize();
	Vector getOffset();
	Type getType();
	bool  getActive();

	void setActive(bool active);
	void setType(Type ht);
	void setOffset(Vector offset);
	void setSize(Vector size);
};

#endif