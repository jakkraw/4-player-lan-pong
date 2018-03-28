#include "time.h"


bool Time::initialize() {
	setMaxFrameRate(Config::time.FRAME_RATE);
	setMinFrameRate(Config::time.MIN_FRAME_RATE);
	setLimitFPS(Config::time.limitFPS);

	startTicks = std::chrono::high_resolution_clock::now();
	return true;
}

void Time::shutdown(){
}

void Time::manageFPS()
{
	stopTicks = std::chrono::high_resolution_clock::now();
	frameTime = std::chrono::duration_cast<std::chrono::microseconds>(stopTicks - startTicks);

	if (getLimitFPS())
	{
		if (frameTime < minFrameTime){
			Sleep(DWORD((minFrameTime - frameTime).count() / 1000.0));
			//std::this_thread::sleep_for(minFrameTime - frameTime);
			
			frameTime = minFrameTime;
		}

		if (frameTime > maxFrameTime){frameTime = maxFrameTime;
		}
	}

	if (frameTime.count() > 0) {
		fps = (fps*0.9) + (100000.0 / (frameTime.count()));
	}

	startTicks = std::chrono::high_resolution_clock::now();
}

void Time::setLimitFPS(bool lfps){
	limitFPS = lfps;
}

bool Time::getLimitFPS(){
	return limitFPS;
}

void Time::setMaxFrameRate(unsigned int fr){
	minFrameTime = std::chrono::microseconds((long long)(1000000.0 / double(fr)));
}

void Time::setMinFrameRate(unsigned int mfr){
	maxFrameTime = std::chrono::microseconds((long long)(1000000.0 / double(mfr)));
}

double Time::getFrameTime() const{
	return frameTime.count()/1000000.0;
}

Uint32 Time::getFPS() const {
	return Uint32(fps);
}
