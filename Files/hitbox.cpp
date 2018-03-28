#include "hitbox.h"

Hitbox::Hitbox()
{
	type = Type::RECTANGLE;
	size = { 1,1 };
	offset = { 0,0 };
	radius = 1.0;
	active = true;
}

bool Hitbox::getActive(){
	return active;
}

void Hitbox::setActive(bool active){
	this->active = active;
}

Hitbox::Type Hitbox::getType() {
	return type;
}

void Hitbox::setType(Type ht){
	type = ht;
}

void Hitbox::setOffset(Vector offset){
	this->offset = offset;
}

Vector Hitbox::getOffset() {
	return offset;
}

void Hitbox::setSize(Vector size) {
	this->size = size;
	this->radius = size.x / 2;
}

Vector Hitbox::getSize() {
	return size;
}

double Hitbox::getRadius() {
	return radius;
}
