#pragma once

#include "pointersInterface.h"
#include "input.h"
#include "entity.h"

class ControllerInterface :public PointersInterface {
private:
	Entity* entity;
public:
	virtual void update() = 0;
	void setEntity(Entity*entity) { this->entity = entity; }
	inline Entity* getEntity() const { return entity; }
};
