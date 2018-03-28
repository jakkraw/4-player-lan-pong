#pragma once
#include "pointersInterface.h"
#include "pluginInterface.h"
#include "engine.h"

class StageInterface : protected PointersInterface,public PluginInterface
{
public:
	virtual void ai() {};
	virtual void collisions() {};
	virtual void render() {};
	virtual void update() {};
	virtual ~StageInterface() {};
};
