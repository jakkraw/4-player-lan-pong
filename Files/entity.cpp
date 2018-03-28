#include "entity.h"

#include "physics.h"

Entity::Entity(){
	setId(generateId());
	texture = new Texture();
	velocity = { 0,0 };
	angularVelocity = 0.0;
	scale = { 1,1 };
}


Entity::Entity(const Entity & other){
	this->scale = other.scale;
	this->setId(other.getId());
	this->velocity = other.velocity;
	this->angularVelocity = other.angularVelocity;
	this->hitbox = other.hitbox;
	
	if (IS_A(other.texture, AnimatedTexture*)) {
		this->texture = new AnimatedTexture(*(AnimatedTexture*)other.texture);
	}
	else {this->texture = new Texture(*other.texture);}
}

Entity & Entity::operator=(const Entity & other){
	this->setId(other.getId());
	this->hitbox = other.hitbox;
	this->velocity = other.velocity;
	this->angularVelocity = other.angularVelocity;
	this->texture->operator= (*other.texture);
	return *this;
}

Entity::~Entity(){
	delete(texture);
}

void Entity::initialize(){
	getTexture()->initialize(graphics()->getSurface(getTexture()->getName()), graphics()->getRenderer());
}


void Entity::setScale(Vector scale)
{
	if (this->scale == scale) return;

	Vector s = { scale.x / this->scale.x,scale.y / this->scale.y };
	this->scale = scale;

	getHitbox()->setOffset(s*getHitbox()->getOffset());
	getHitbox()->setSize(s *getHitbox()->getSize());
	texture->setRotCenter(s* texture->getRotCenter());
	texture->setSize(s*texture->getSize());
	
	
}

void Entity::setSize(Vector size){
	setScale(size / getTexture()->getSize());
}

Vector Entity::getScale() const{
	return scale;
}


Point Entity::getPosition() const{
	return texture->getPosition();
}

Point Entity::getVelocity() const{
	return velocity;
}

double Entity::getAngularVelocity() const{
	return angularVelocity;
}


double Entity::getAngle() const{
	return texture->getAngle();
}


Texture * Entity::getTexture() const{
	return texture;
}

Hitbox* Entity::getHitbox(){
	return &hitbox;
}


void Entity::setHitbox(Hitbox hitbox){
	this->hitbox = hitbox;
}


void Entity::collide(Entity * ent){
	physics()->collide(this, ent);
}

void Entity::setPosition(Point p){
	texture->setPosition(p);
}



void Entity::setVelocity(Vector p){
	velocity = p;
}


void Entity::setAngularVelocity(double v){
	angularVelocity = v;
}

void Entity::setTexture(Texture texture){
	delete this->texture;
	this->texture = new Texture(texture);
}

void Entity::setTexture(AnimatedTexture texture){
	delete this->texture;
	this->texture = new AnimatedTexture(texture);
}

void Entity::setAngle(double angle){
	this->texture->setAngle(angle);
}

void Entity::update(){
	
	texture->update(time()->getFrameTime());
	texture->setPosition(texture->getPosition() + velocity*time()->getFrameTime());
	texture->setAngle(texture->getAngle() + angularVelocity*time()->getFrameTime());
}


void Entity::handleCollision(Entity * copy, Entity * original){
	
	Hitbox::Type aType = this->getHitbox()->getType();
	Hitbox::Type bType = copy->getHitbox()->getType();

	if (aType == bType){
		if (aType == Hitbox::RECTANGLE) {
			physics()->defaultRectRectCollision(this, copy);
		}
		else if (aType == Hitbox::CIRCLE) {
			physics()->defaultCircleCircleCollision(this, copy);
		}
	}
	else if (aType == Hitbox::CIRCLE && bType == Hitbox::RECTANGLE) {
		physics()->defaultRectCircleCollision(copy, this);
	}
	else if (bType == Hitbox::CIRCLE && aType == Hitbox::RECTANGLE) {
		physics()->defaultRectCircleCollision(this, copy);
	}


}


void Entity::draw()
{

	graphics()->drawTexture(texture);

#if defined _DEBUG && defined ENTITY_DEBUG

	Point pos = texture->getPosition();
	Point hPos = pos + getHitbox()->getOffset();
	Point center = texture->getRotCenter() + pos;
	Vector hSize = getHitbox()->getSize();
	deg angle = texture->getAngle();

	//offset 
	int size = 6;
	graphics()->setColor(color::BLACK);
	graphics()->drawRect(Rect(hPos.x - size / 2, hPos.y - size / 2, size, size));
	//

	if (!getHitbox()->getActive()) return;


	Point p1 = hPos;
	Point p2 = Point(hPos.x + hSize.x, hPos.y);
	Point p3 = Point(hPos.x, hPos.y + hSize.y);
	Point p4 = Point(hPos.x + hSize.x, hPos.y + hSize.y);

	p1 = math::rotatePointByAngle(p1, center, angle);
	p2 = math::rotatePointByAngle(p2, center, angle);
	p3 = math::rotatePointByAngle(p3, center, angle);
	p4 = math::rotatePointByAngle(p4, center, angle);

	graphics()->setColor(color::BLACK);
	graphics()->drawLine(p1, p2);
	graphics()->drawLine(p1, p3);
	graphics()->drawLine(p2, p4);
	graphics()->drawLine(p3, p4);


	Texture t;
	t.debug = false;
	t.setPosition(hPos);
	t.setSize(hSize);
	t.setRotCenter(texture->getRotCenter()-getHitbox()->getOffset());
	t.setAngle(angle);

	if (getHitbox()->getType() == Hitbox::RECTANGLE){
		t.initialize(graphics()->getSurface("hitbox_square.png"), graphics()->getRenderer());
	}

	if (getHitbox()->getType() == Hitbox::CIRCLE){
		t.initialize(graphics()->getSurface("hitbox_circle.png"), graphics()->getRenderer());
	}

	graphics()->drawTexture(&t);

	
	graphics()->getText()->changeFontColor(color::BLACK);
	graphics()->getText()->changeFontSize(12);
	Point text_pos = Point(pos.x, pos.y + getTexture()->getSize().y);
	std::string s = "(" + std::to_string(int(velocity.x)) + "," + std::to_string(int(velocity.y)) + ") # " + std::to_string(int(getAngularVelocity()));
	graphics()->drawText(s, int(text_pos.x), int(text_pos.y));

	

#endif
}