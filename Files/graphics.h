#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL_image.h"
#include "SDL.h"
#include "resourceManager.h"
#include "texture.h"
#include "animatedTexture.h"
#include "text.h"
#include "color.h"
#include "logger.h"
#include "config.h"
#include <string>
#include <vector>
#include "pluginInterface.h"

class Graphics : public PluginInterface
{
private:
	mixcolor backgroundColor;
	SDL_DisplayMode currentDisplayInfo;
	SDL_Rect currentDisplayBounds;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Text* text;
	ResourceManager<SDL_Surface*>* surfaces;
	int rendererQuality = 2;
	Vector screenSize; 

public:
	bool initialize();
	void shutdown();
	void startDraw() const;
	void endDraw() const;

	void setDisplayMode(int n);
	void setFullscreen(bool f);
	bool setWindowSize(int w, int h);
	bool setDisplay(int n);
	void setScreenSize(int w, int h);
	void setColor(mixcolor color) const;
	void setBackgroundColor(mixcolor color);

	SDL_Surface* getSurface(std::string surfName) const;
	Text* getText() const; 
	SDL_Renderer* getRenderer() const;
	bool getFullscreen() const;
	Vector getScreenSize() const;
	Vector getWindowSize() const;

	void drawPoint(int x, int y) const;
	void drawPoint(Point p) const;
	void drawLine(int x, int y, int x2, int y2) const;
	void drawLine(Point a, Point b) const;
	void drawRect(int x, int y, int w, int h) const;
	void drawRect(Rect r) const;
	void drawSurface(SDL_Surface* surface, SDL_Rect* src = NULL, SDL_Rect* dst = NULL) const;
	void drawTexture(Texture* texture) const;
	void drawText(std::string str,int x,int y) const;
};

#endif