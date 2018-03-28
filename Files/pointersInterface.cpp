#include "PointersInterface.h"
#include "engine.h"
Pointers PointersInterface::globalPointers;

void PointersInterface::deleteGlobalPointers() {
	SAFE_DELETE(globalPointers.input);
	SAFE_DELETE(globalPointers.graphics);
	SAFE_DELETE(globalPointers.time);
	SAFE_DELETE(globalPointers.audio);
	SAFE_DELETE(globalPointers.physics);
	SAFE_DELETE(globalPointers.network);
}

PointersInterface::PointersInterface() :localPointers(&globalPointers) {}

PointersInterface::~PointersInterface() {
	if (localPointers != &globalPointers)delete localPointers;
}

void PointersInterface::resetPointers()
{
	if (localPointers != &globalPointers)delete localPointers;
	localPointers = &globalPointers;
}

void PointersInterface::setLocalPointers(Pointers pointers) {
	if (localPointers != &globalPointers)delete localPointers;
	localPointers = new Pointers(pointers);
}
