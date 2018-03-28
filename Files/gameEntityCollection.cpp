#include "gameEntityCollection.h"



Wall * GameEntityCollection::get(Wall::Type position) {
	return (Wall*)EntityCollection::get(wm.get(position));
}

Player* GameEntityCollection::get(Player::Position pos) {
	return pm.get(pos);
}

Entity * GameEntityCollection::get(id id) {
	return EntityCollection::get(id);
}


void GameEntityCollection::update()
{
	EntityCollection::update();
	pm.update();


}

void GameEntityCollection::render()
{
	EntityCollection::render();


	for (auto &p : pm.players) {
		if (p != nullptr&& p->getPaddle()) {
			Point pos = p->getPaddle()->getPosition();
			pos = pos + p->getPaddle()->getTexture()->getRotCenter() + Point(20, -40);
			graphics()->drawText(std::to_string(p->points), int(pos.x), int(pos.y));
		}


	}

}

void GameEntityCollection::texUpdate()
{
	for (auto &i : collection) {
		if (IS_A(i.second->getTexture(), AnimatedTexture*)){
			i.second->update();
		}
	}

}



void GameEntityCollection::createBoard()
{
	add(Wall::UP);
	add(Wall::DOWN);
	add(Wall::LEFT);
	add(Wall::RIGHT);
	add(Wall::LEFT_LOWER);
	add(Wall::LEFT_UPPER);
	add(Wall::RIGHT_LOWER);
	add(Wall::RIGHT_UPPER);
}


Ball* GameEntityCollection::addBall()
{
	Ball* ball = bm.createBall();
	add(ball);
	return ball;
}

void GameEntityCollection::add(Wall::Type position){
	Wall * wall = wm.createWall(position);
	wm.add(wall);
	add(wall);
}

void GameEntityCollection::add(Entity *entity)
{
	entity->setLocalPointers(getLocalPointers());
	if (graphics())entity->initialize();

	if (IS_A(entity, Wall*)) {wm.add((Wall*)entity);}
	if (IS_A(entity, Ball*)) { bm.add((Ball*)entity); }
	EntityCollection::add(entity);
	
}

void GameEntityCollection::add(Player *player)
{
	if (pm.size() >= 4) return;
	pm.add(player);
	add(player->getPaddle());
}

void GameEntityCollection::remove(id id)
{
	wm.remove(id);
	bm.remove(id);
	EntityCollection::remove(id);
}

void GameEntityCollection::remove(Wall::Type position){
	remove(wm.get(position));
	wm.remove(position);

}

void GameEntityCollection::remove(Player *player){
	add((Wall::Type)player->getPosition());
	pm.remove(player);
	remove(player->getPaddle()->getId());

}

void GameEntityCollection::clear()
{
	pm.clear();
	bm.clear();
	EntityCollection::clear();
}
