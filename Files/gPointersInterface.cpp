#include "gPointersInterface.h"
#include "game.h"
GamePointers GPointersInterface::globalGamePointers;

GamePointers * GPointersInterface::getGlobalGamePointers() {
	return &globalGamePointers;
}


void GPointersInterface::deleteGlobalGamePointers()
{
	SAFE_DELETE(globalGamePointers.console);
}

GPointersInterface::GPointersInterface() :localGamePointers(&globalGamePointers) {}

GPointersInterface::~GPointersInterface() {
	if (localGamePointers != &globalGamePointers)
		delete localGamePointers;
}

void GPointersInterface::setLocalGamePointers(GamePointers pointers) {
	if (localGamePointers != &globalGamePointers)
		delete localGamePointers;
	localGamePointers = new GamePointers(pointers);
}

