#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "SDL.h"


template<class... Args>
void Log(const std::string& format, Args&& ... args)
{
#if defined _DEBUG
	SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_VERBOSE);
	SDL_Log(format.c_str(), std::forward<Args>(args)...);
#endif
}

template<class... Args>
void LogError(const std::string& format, Args&& ... args)
{
#if defined _DEBUG
	SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_VERBOSE);
	std::string message = format;
		SDL_LogError(SDL_LOG_CATEGORY_CUSTOM,
			message.c_str(), std::forward<Args>(args)...);
#endif
}

template<class... Args>
void LogCritical(const std::string& format, Args&& ... args)
{
#if defined _DEBUG
	SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_VERBOSE);
	std::string message = format;
		message += " SDL Error: %s\n";
		SDL_LogCritical(SDL_LOG_CATEGORY_CUSTOM,
			message.c_str(), std::forward<Args>(args)..., SDL_GetError());
#endif
}

#endif 