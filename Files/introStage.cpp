#include "introStage.h"

bool IntroStage::initialize()
{
	audio()->playSong("volatile.mp3");
	
	graphics()->setBackgroundColor(color::BLACK);
	graphics()->setScreenSize(800, 600);

	janTexture = new Texture();
	kasiaTexture = new Texture();
	kubaTexture = new Texture();
	rafalTexture = new Texture();
	
	ball = new Ball();
	paddle1 = new Paddle();
	paddle2 = new Paddle();

	ball->initialize();

	paddle1->initialize();
	paddle1->setPosition({ 220 - paddle1->getTexture()->getSize().x, 100 });

	paddle2->initialize();
	paddle2->setPosition({ 580, 100 });


	etiTexture.initialize(graphics()->getSurface("eti.png"), graphics()->getRenderer());
	etiTexture.setPosition(Point(graphics()->getScreenSize().x, graphics()->getScreenSize().y)/3);
	etiTexture.setSize(Point(graphics()->getScreenSize().x / 3, graphics()->getScreenSize().y / 3));

	janTexture->initialize(graphics()->getSurface("jan.png"), graphics()->getRenderer());
	janTexture->setPosition({ 500, 500 });
	janTexture->setSize(janTexture->getSource().size() / 4);
	
	kasiaTexture->initialize(graphics()->getSurface("kasia.png"), graphics()->getRenderer());
	kasiaTexture->setPosition({ 500, 520 });
	kasiaTexture->setSize(kasiaTexture->getSource().size()/4);
	
	kubaTexture->initialize(graphics()->getSurface("kuba.png"), graphics()->getRenderer());
	kubaTexture->setPosition({ 500, 540 });
	kubaTexture->setSize(kubaTexture->getSource().size() / 4);
	
	rafalTexture->initialize(graphics()->getSurface("rafal.png"), graphics()->getRenderer());
	rafalTexture->setPosition({ 500, 560 });
	rafalTexture->setSize(rafalTexture->getSource().size() / 4);


	ball->setPosition({ 300, 300 });
	ball->setVelocity(Vector(20,20));
	return true;
}

void IntroStage::render()
{
	graphics()->drawTexture(&etiTexture);

	paddle1->draw();
	paddle2->draw();
	ball->draw();

	graphics()->drawTexture(janTexture);
	graphics()->drawTexture(kasiaTexture);
	graphics()->drawTexture(kubaTexture);
	graphics()->drawTexture(rafalTexture);

}

void IntroStage::update()
{
	paddle1->update();
	paddle2->update();
	ball->update();

	paddle1->setAngularVelocity(120);
	paddle2->setAngularVelocity(-150);

	if (input()->keyPressed(SDLK_RETURN)){
				engine()->setStage(new SinglePlayerStage());
	}

}


void IntroStage::shutdown()
{
	delete	janTexture;
	delete	kasiaTexture;
	delete	kubaTexture;
	delete	rafalTexture;
	delete	ball;
	delete	paddle1;
	delete	paddle2;

}
