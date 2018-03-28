#ifndef TIME_H
#define TIME_H

#include "logger.h"
#include "config.h"
#include "pluginInterface.h"
#include <chrono>
#include <thread>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::microseconds microseconds;

class Time : public PluginInterface
{
private:
	bool limitFPS;
	Clock::time_point startTicks;
	Clock::time_point stopTicks;
	microseconds maxframeRate;
	microseconds minFrameRate;
	microseconds minFrameTime;
	microseconds maxFrameTime;
	microseconds frameTime;
	double fps;
		
public:
	bool initialize();
	void shutdown();
	void manageFPS();
	
	void setLimitFPS(bool lfps);
	bool getLimitFPS();
	double getFrameTime() const;
	Uint32 getFPS() const;
	void setMaxFrameRate(unsigned int fr);
	void setMinFrameRate(unsigned int mfr);

};

#endif








