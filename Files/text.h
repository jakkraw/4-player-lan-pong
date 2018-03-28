#ifndef TEXT_H
#define TEXT_H

#include "SDL_ttf.h"
#include "color.h"
#include "texture.h"
#include "logger.h"
#include <string>
#include "pluginInterface.h"

namespace TextNS{
	const std::string FONT_PATH = "./Assets/Fonts/";
	const std::string FONT_NAME = "simple.ttf";
}

class Text : public PluginInterface
{
	int fontSize = 12;
	SDL_Color _color = toSDLColor(color::BLACK);
	TTF_Font* font;
	std::string fontName = TextNS::FONT_NAME;
public:
	bool initialize();
	void shutdown();
	Texture* textToTexture(std::string t,SDL_Renderer* renderer);
	void changeFontSize(int s);
	void changeFontColor(color color);
	bool changeFont(std::string path);
	int getFontSize();

};

#endif