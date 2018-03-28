#include "msg.h"


#define MSG(a) #a,
char *names[] = {
	MSG_ENUM
};
#undef MSG


const char * Msg::toString() {
	return names[type];
}


EntityInfoMsg::EntityInfoMsg(Entity * entity) :UdpMsg(EntityInfo, entity->getId(), sizeof(EntityInfoMsg)) {
	if (entity) {
		hitbox = *entity->getHitbox();
		texture = *entity->getTexture();
		velocity = entity->getVelocity();
		angularVelocity = entity->getAngularVelocity();
		scale = entity->getScale();
	}

}



void EntityInfoMsg::setEntity(Entity * entity) {
	if (entity) {
		entity->setVelocity(velocity);
		entity->setScale(scale);
		entity->setAngularVelocity(angularVelocity);
		entity->setHitbox(hitbox);
		entity->getTexture()->operator=(texture);
	}
}




AddEntityMsg::AddEntityMsg(Entity * entity) : Msg(AddEntity, sizeof(AddEntityMsg)), eimsg(entity) {
	if (IS_A(entity, ::Paddle*)) { type = Type::Paddle; }
	else if (IS_A(entity, ::Ball*)) { type = Type::Ball; }
	else if (IS_A(entity, ::Wall*)) { type = Type::Wall; }
	else type = Type::None;
}

Entity * AddEntityMsg::createEntity() {
	Entity* entity = nullptr;
	switch (type) {
	case Paddle:
		entity = new ::Paddle();
		break;
	case Ball:
		entity = new ::Ball();
		break;
	case Wall:
		entity = new ::Wall();
		break;
	case None:
		LogError("ADDENTITYMSG NONE MSG !");
		return nullptr;
	}

	entity->setId(eimsg.getId());
	eimsg.setEntity(entity);

	return entity;
}

PlayerPaddleMsg::PlayerPaddleMsg(Player * player, ::id to) :Msg(PlayerPaddle, sizeof(PlayerPaddleMsg)) {
	this->to = to;
	this->paddleId = player->getPaddle()->getId();
	this->position = player->getPosition();
}

RemoveEntityMsg::RemoveEntityMsg(::id id) : Msg(RemoveEntity, sizeof(RemoveEntityMsg)) {
	this->paddle_id = id;
}

CreatePlayerMsg::CreatePlayerMsg(Player *player) : Msg(CreatePlayer, sizeof(CreatePlayerMsg)) {
	playerName.set(player->getName());
	surfaceName.set(player->getPaddle()->getTexture()->getName());
	scale = player->getPaddle()->getScale();
}