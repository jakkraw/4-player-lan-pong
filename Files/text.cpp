#include "text.h"


bool Text::initialize()
{
	if (TTF_Init() == -1){
		LogError(
			"SDL_ttf could not initialize! SDL_ttf Error: %s\n",
			TTF_GetError());
		return false;
	}

	changeFont(fontName);

	return true;
}

void Text::shutdown()
{
	TTF_CloseFont(font);
	TTF_Quit();
}

Texture * Text::textToTexture(std::string text, SDL_Renderer * renderer)
{
	if (text.empty()) text = " ";

	//SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(),color, wraplenghtPixels);
	//SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(),color );
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), _color);

	if (!textSurface) {
		LogError("Unable to render text surface! SDL_ttf Error: %s\n",TTF_GetError());
		SDL_FreeSurface(textSurface);
		return nullptr;
	}

	Texture* tex = new Texture();

	if (!tex->initialize(textSurface, renderer))
	{
		LogError("Unable to create texture from text surface! SDL_ttf Error: %s\n",TTF_GetError());
		SDL_FreeSurface(textSurface);
		delete tex;
		return nullptr;
	}

	SDL_FreeSurface(textSurface);
	tex->setSize(tex->getSource().size());
	tex->debug = false;

	return tex;
}


void Text::changeFontSize(int s)
{
	if (fontSize == s)	return;

	TTF_Font* tmp = font;
	font = TTF_OpenFont((TextNS::FONT_PATH+fontName).c_str(), s);
	if (!font) {
		LogError(
			"Font Resize to : %i|TTF_OpenFont: %s\n -> Loading Previous Font",
				s,TTF_GetError());
		TTF_CloseFont(font);
		font = tmp;
		return;
	}

	fontSize = s;
	TTF_CloseFont(tmp);

}

void Text::changeFontColor(color c){
	_color = toSDLColor(c);
}

bool Text::changeFont(std::string fontName)
{
	this->fontName = fontName;
	TTF_Font* tmp = font;
	font = TTF_OpenFont((TextNS::FONT_PATH + fontName).c_str(), fontSize);
	if (!font) 
	{
		LogError(
			"TTF_OpenFont: %s\n -> loading previous font",
			TTF_GetError());
		TTF_CloseFont(font);
		font = tmp;
		return false;
	}

	TTF_CloseFont(tmp);
	return true;
}

int Text::getFontSize(){
	return fontSize;
}
