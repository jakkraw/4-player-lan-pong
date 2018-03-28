#include "physics.h"
#include "entity.h"
#include "wall.h"
#include "macro.h"

bool Physics::initialize(){
	copyB = new Entity();
	copyA = new Entity();
	return true;
}

void Physics::shutdown()
{
	SAFE_DELETE(copyA);
	SAFE_DELETE(copyB);
}

bool Physics::collide(Entity * a, Entity * b)
{
	if (!a || !b) return false;
	if (!a->getHitbox()->getActive() || !b->getHitbox()->getActive()) return false;

	Hitbox::Type aType = a->getHitbox()->getType();
	Hitbox::Type bType = b->getHitbox()->getType();
	bool colided = false;
	if (IS_A(a, Wall*) && IS_A(b,Wall*)){
		return false;
	}
	if (aType == bType)
	{
		
		if (aType == Hitbox::RECTANGLE) {
			if (colided = collideRectRect(a, b)) {
				unstuckRectRect(a, b);
			}				
		}
		else if (aType == Hitbox::CIRCLE) {
			if (colided = collideCircleCircle(a, b)) {
				unstuckCircleCircle(a, b);
			}	
		}		
	}

	else if (aType == Hitbox::CIRCLE && bType == Hitbox::RECTANGLE){
		if ((colided = collideRectCircle(b, a))){
			unstuckRectCircle(b, a);
		}	
	}
	else if (bType == Hitbox::CIRCLE && aType == Hitbox::RECTANGLE){
		if ((colided = collideRectCircle(a, b))) {
			unstuckRectCircle(a, b);
		}	
	}

	if (colided){
		*copyB = *b;
		*copyA = *a;
		a->handleCollision(copyB,b);
		b->handleCollision(copyA,a);
	}

	return colided;
}


bool Physics::defaultCircleCircleCollision(Entity * circleA, Entity * circleB)
{
	Point aPos = circleA->getPosition() + circleA->getTexture()->getRotCenter();
	double aRad = circleA->getHitbox()->getRadius();
	Vector aVel = circleA->getVelocity();

	Point bPos = circleB->getPosition() + circleB->getTexture()->getRotCenter();
	double bRad = circleB->getHitbox()->getRadius();
	Vector bVel = circleB->getVelocity();

	Line l(aPos, bPos);
	Point aProstPoint = math::intersectPoint(math::createLineNormal(l, aPos + aVel), l);
	Point bProstPoint = math::intersectPoint(math::createLineNormal(l, bPos + bVel), l);

	int aDir = 1;
	int bDir = 1;

	if (aProstPoint.x >= aPos.x) aDir = -1;
	if (bProstPoint.x >	bPos.x) bDir = -1;

	double aLen = math::distance(aPos, aProstPoint);
	double bLen = math::distance(bPos, bProstPoint);

	double diff = abs(aLen*aDir - bLen*bDir);

	
	double aDiff = (diff* bRad / (aRad + bRad)) * 2;
	Point aAddedVector = math::vectorFromAngleAndLength(atan(l.a), aDiff	*aDir);
	circleA->setVelocity(aAddedVector + aVel);

	double bDiff = (diff* aRad / (aRad + bRad)) * 2;
	Point bAddedVector = math::vectorFromAngleAndLength(atan(l.a), bDiff	*bDir);
	circleB->setVelocity(bAddedVector + bVel);

	return true;

}

bool Physics::defaultRectCircleCollision(Entity * rectA, Entity * circleB)
{
	Point bPos = circleB->getPosition() + circleB->getTexture()->getRotCenter();
	Point bVel = circleB->getVelocity();

	// znajdz najblizsza prosta
	bool foundLine = false;
	Point intersectPoint;


	double min = INT_MAX;
	for (int i = 0; i < 4; i++)
	{
		Line l = lines[i];
		double dist = math::distanceFromPointToLine(bPos,l);
		if (min >= dist)
		{
			Point iP = math::intersectPoint(l, math::createLineNormal(l, bPos));
			if (math::isValueBetween(iP.x, l.p1.x, l.p2.x) &&
				math::isValueBetween(iP.y, l.p1.y, l.p2.y)) {

				min = dist;
				foundLine = true;
				intersectPoint = iP;
			}
		}
	}

	//jesli nie znaleziono znajdz najblizszy rog
	if (!foundLine){
		double min = INT_MAX;
		for (int i = 0; i < 4; i++)
		{
			if (min > math::distance(lines[i].p1, bPos)){
				min = math::distance(lines[i].p1, bPos);
				intersectPoint = lines[i].p1;
			}
		}
	}


	//odbij
	Vector kierunek = bPos - intersectPoint;
	kierunek = kierunek.getUnitVector() * 2 * bVel.length();

	Vector wypadkowa = kierunek  + bVel;
	wypadkowa = wypadkowa.getUnitVector() *  bVel.length();

	circleB->setVelocity(wypadkowa);

	return true;
}

bool Physics::defaultRectRectCollision(Entity * rectA, Entity * rectB){
	rectA->setVelocity( rectA->getVelocity()*(-1)/2);
	rectA->setAngularVelocity(0);
	rectB->setVelocity(rectB->getVelocity()*(-1) / 2);
	rectB->setAngularVelocity(0);

	return true;
}


bool Physics::collideRectRect(Entity * rectA, Entity * rectB){
	deg aAngle = rectA->getAngle();
	Vector aHSize = rectA->getHitbox()->getSize();
	Point aHPos = rectA->getPosition() + rectA->getHitbox()->getOffset();
	Point aCenter = rectA->getTexture()->getRotCenter() + rectA->getPosition();

	Point pointsA[4];
	Line linesA[4];
	math::rectanglePointsLines(pointsA, linesA, aHPos, aHSize, aCenter, aAngle);


	deg bAngle = rectB->getAngle();
	Vector bHSize = rectB->getHitbox()->getSize();
	Point bHPos = rectB->getPosition() + rectB->getHitbox()->getOffset();
	Point bCenter = rectB->getTexture()->getRotCenter() + rectB->getPosition();

	Point pointsB[4];
	Line linesB[4];
	math::rectanglePointsLines(pointsB, linesB, bHPos, bHSize, bCenter, bAngle);

	for (int i = 0; i < 4; i++){
		if (math::pointInsideLines(linesA, &pointsB[i]))
			return true;
	}

	for (int i = 0; i < 4; i++){
		if (math::pointInsideLines(linesB, &pointsA[i]))
			return true;
	}

	return false;
}

bool Physics::collideCircleCircle(Entity * circleA, Entity * circleB)
{
	Point aCenter = circleA->getPosition() + circleA->getTexture()->getRotCenter();
	double aRad = circleA->getHitbox()->getRadius();

	Point bCenter = circleB->getPosition() + circleB->getTexture()->getRotCenter();
	double bRad = circleB->getHitbox()->getRadius();

	Vector diff( abs(aCenter.x - bCenter.x) , abs(aCenter.y - bCenter.y) );

	if (diff.length() < aRad + bRad){
		return true;
	}
	
	return false;		
}

bool Physics::collideRectCircle(Entity * rectA, Entity * circleB)
{
	deg aAngle = rectA->getAngle();
	Vector aHSize = rectA->getHitbox()->getSize();
	Point aHPos = rectA->getPosition() + rectA->getHitbox()->getOffset();
	Point aCenter = rectA->getTexture()->getRotCenter() + rectA->getPosition();

	Point bCenter = circleB->getPosition() + circleB->getTexture()->getRotCenter();
	double bRad = circleB->getHitbox()->getRadius();

    //jesli nawet nie warto sprawdzac
	Vector diff (abs(aCenter.x - bCenter.x) , abs(aCenter.y - bCenter.y) );
	if (diff.length() > aHSize.length()/2 +bRad) 
		return false;
	
	//jesli srodek paletki w okregu
	if (diff.length() < bRad) 
		return true;

	Point points[4];
	math::rectanglePointsLines(points, lines, aHPos, aHSize, aCenter, aAngle);

	// Sprawdza przeciecia
	for (int i = 0; i<4; i++) {
		if (math::checkIntersections(lines[i], bCenter, bRad)) return true;
	}
    
	return math::pointInsideLines(lines, &bCenter);
}


bool Physics::unstuckRectRect(Entity * rectA, Entity * rectB)
{
	int counter = 0;
	double ft = 0.001;
	while (collideRectRect(rectA, rectB)){
		rectB->setPosition(rectB->getPosition() - rectB->getVelocity()*ft);
		rectA->setPosition(rectA->getPosition() - rectA->getVelocity()*ft);
		rectA->setAngle(rectA->getAngle() - rectA->getAngularVelocity() *ft);
		rectB->setAngle(rectB->getAngle() - rectB->getAngularVelocity() *ft);


		counter++;
		if (counter > 100000) break;
	}
	return true; Log("kolizja");
	return false;
}

bool Physics::unstuckRectCircle(Entity * rectA, Entity * circleB)
{
	int counter = 0;
	double ft = 0.001;
	while (collideRectCircle(rectA, circleB)){
		circleB->setPosition(circleB->getPosition() - circleB->getVelocity()*ft);
		rectA->setPosition(rectA->getPosition() - rectA->getVelocity()*ft);
		rectA->setAngle(rectA->getAngle() - rectA->getAngularVelocity() *ft);


		counter++;
		if (counter > 100000) break;
	}
	return true;
}

bool Physics::unstuckCircleCircle(Entity * circleA, Entity * circleB)
{
	int counter = 0;
	double ft = 0.001;
	while (collideCircleCircle(circleA, circleB)) {
		circleB->setPosition(circleB->getPosition() - circleB->getVelocity()*ft);
		circleA->setPosition(circleA->getPosition() - circleA->getVelocity()*ft);
		counter++;
		if (counter > 100000) break;
	}
	return true;

}

