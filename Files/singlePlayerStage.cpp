#include "singlePlayerStage.h"

#include "game.h"



bool SinglePlayerStage::initialize()
{
	graphics()->setBackgroundColor(color::LIGHT_BLUE);
	graphics()->setScreenSize(600, 600);
	ec.createBoard();

	Ball* ball1 = new Ball();
	ball1->setVelocity(Vector(-30, 71) * 4);
	ball1->setPosition({ 200, 200 });
	ec.add(ball1);

	Ball* ball2 = new Ball();
	ball2->setVelocity(Vector(150, 23) * 3);
	ball2->setPosition({ 300, 100 });
	ec.add(ball2);

	
	ec.add(&p1);
	ec.add(&p2);
	ec.add(&p3);
	ec.add(&p4);
	ec.remove((Wall::Type)p1.getPosition());
	return true;
}

void SinglePlayerStage::render()
{
	ec.render();
}

void SinglePlayerStage::update()
{
	ec.update();
}


void SinglePlayerStage::collisions()
{
	ec.collisions();
}

void SinglePlayerStage::shutdown()
{
}