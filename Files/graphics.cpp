#include "graphics.h"



bool Graphics::initialize()
{
	
	SDL_Init(SDL_INIT_VIDEO);
	setDisplayMode(0);

	int windowX = SDL_WINDOWPOS_CENTERED;
	int windowY = SDL_WINDOWPOS_CENTERED;

	if (Config::graphics.windowX != -1 && Config::graphics.windowY != -1) {
		windowX = Config::graphics.windowX;
		windowY = Config::graphics.windowY;
	}

		window = SDL_CreateWindow("Game", 
			windowX,
			windowY,
			Config::graphics.windowWidth,
			Config::graphics.windowHeight,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	
	

	if (!window) {
		LogCritical("Window could not be created!");
		SDL_DestroyWindow(window);
		return false;
	}
	setFullscreen(Config::graphics.fullscreen);

	renderer = SDL_CreateRenderer(window, -1, Config::graphics.rendererType);
	if (!renderer) {
		LogCritical("Renderer could not be created!");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return false;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer, int(screenSize.x), int(screenSize.y));
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, std::to_string(rendererQuality).c_str());
	

	if (!IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG)) { 
		LogCritical("SDL_image could not initialize!"); 
		return false; 
	}

	text = new Text();
	if (!text->initialize()) return false;

	surfaces = new ResourceManager<SDL_Surface*>(&IMG_Load, &SDL_FreeSurface);
	surfaces->setName("Surfaces");
	std::regex regex("^(.*).\\.(jpg|png|jpeg|gif)$", std::regex_constants::icase);
	surfaces->loadMatchingFiles("./Assets/", regex);
	
	return true;
}

void Graphics::shutdown(){
	text->shutdown();
	delete(text);
	delete(surfaces);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_VideoQuit();
}


SDL_Surface * Graphics::getSurface(std::string surfName) const {
	return surfaces->get(surfName);
}

void Graphics::startDraw() const{
	setColor(backgroundColor);
	drawRect(Rect(0, 0, getScreenSize().x, getScreenSize().y));
}

void Graphics::endDraw() const{
	SDL_RenderPresent(renderer);
	setColor(color::BLACK);
}

void Graphics::setColor(mixcolor color) const{
	SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
}

void Graphics::setBackgroundColor(mixcolor color){
	this->backgroundColor = color;
}

Text* Graphics::getText() const{
	return text; 
}

SDL_Renderer* Graphics::getRenderer() const{ 
	return renderer; 
}

bool Graphics::setWindowSize(int w, int h){
	SDL_SetWindowSize(window,w, h);
	Config::graphics.windowWidth = w;
	Config::graphics.windowHeight = h;
	return true;
}

bool Graphics::getFullscreen() const{
	return	Config::graphics.fullscreen;
}

Vector Graphics::getScreenSize() const{
	return screenSize;
}

Vector Graphics::getWindowSize() const{
	return Vector(Config::graphics.windowWidth, Config::graphics.windowHeight);
}



bool Graphics::setDisplay(int n){
	SDL_DisplayMode tmpDisp;
	if (n >= SDL_GetNumVideoDisplays()){
		return false;
	}
	int error = SDL_GetCurrentDisplayMode(n, &tmpDisp);
	if (error != 0)
		Log("Could not get display mode for video display #%d: %s", n, SDL_GetError());
	else{
		Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", n, tmpDisp.w, tmpDisp.h, tmpDisp.refresh_rate);
		currentDisplayInfo = tmpDisp;
		SDL_GetDisplayBounds(n, &currentDisplayBounds);
		SDL_SetWindowPosition(window,
			int(currentDisplayBounds.x + (currentDisplayBounds.w - screenSize.x) / 2),
			int(currentDisplayBounds.y + (currentDisplayBounds.h - screenSize.y) / 2));
	}
	return true;
}

void Graphics::setDisplayMode(int n){
	if (SDL_GetNumVideoDisplays() < n) return;

	int error = SDL_GetCurrentDisplayMode(n, &currentDisplayInfo);
	if (error != 0)
		Log("Could not get display mode for video display #%d: %s", n, SDL_GetError());
	else
		Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", n, currentDisplayInfo.w, currentDisplayInfo.h, currentDisplayInfo.refresh_rate);

}

void Graphics::setFullscreen(bool f){
	Config::graphics.fullscreen = f;
	if (Config::graphics.fullscreen){
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else{
		SDL_SetWindowFullscreen(window, 0);
	}
}

void Graphics::setScreenSize(int w, int h){
	screenSize = { double(w),double(h) };
	SDL_RenderSetLogicalSize(renderer, int(screenSize.x), int(screenSize.y));
	SDL_RenderClear(renderer);
}



void Graphics::drawPoint(int x, int y) const{
	SDL_RenderDrawPoint(renderer, x, y);
}

void Graphics::drawLine(int x, int y, int x2, int y2) const{
	SDL_RenderDrawLine(renderer, x, y, x2,y2);
}

void Graphics::drawLine(Point a, Point b) const{
	drawLine(int(a.x), int(a.y), int(b.x), int(b.y));
}

void Graphics::drawRect(int x, int y, int w, int h) const{
	SDL_RenderFillRect(renderer, &(SDL_Rect() = { int(x),int(y),int(w),int(h) }));
}

void Graphics::drawRect(Rect r) const{
	drawRect(int(r.x), int(r.y), int(r.w), int(r.h));
}

void Graphics::drawPoint(Point p) const{
	drawPoint(int(p.x), int(p.y));
}


void Graphics::drawSurface(SDL_Surface * surface, SDL_Rect * src, SDL_Rect * dst) const
{
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) { 
		LogError("Unable to create texture from %s! SDL Error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);		
		return;
	}

	SDL_Rect ndst = { int(dst->x), int(dst->y), int(dst->w), int(dst->h) };

	SDL_RenderCopy(renderer, texture, src, dst);
	SDL_DestroyTexture(texture);
}

void Graphics::drawTexture(Texture* t) const
{
	if (!t->getVisible()) return;

	SDL_RenderCopyEx(renderer,
		t->getSDLTexture(),
		&((SDL_Rect)t->getSource()),
		&((SDL_Rect)t->getInfo()),
		t->getAngle(),
		&((SDL_Point)t->getRotCenter()),
		t->getFlip());


	#if defined _DEBUG && defined TEXTURE_DEBUG

		if (!t->debug) return;

		Point p = t->getPosition();
		Point s = t->getSize();
		Point r = t->getRotCenter();
		Point rotPoint = t->getRotCenter() + t->getPosition();
		double angle = t->getAngle();

	
		//Draw Rotation Center
		int sizeRC = 8;
		Rect rotCenterRect;
		rotCenterRect.x = r.x+p.x- sizeRC /2;
		rotCenterRect.y = r.y+p.y- sizeRC /2;
		rotCenterRect.w = sizeRC;
		rotCenterRect.h = sizeRC;
		setColor(color::RED);
		drawRect(rotCenterRect);
		//

		//Draw Info
		Point p1 = p;
		Point p2 = Point(p.x + s.x, p.y);
		Point p3 = Point(p.x , p.y + s.y);
		Point p4 = Point(p.x + s.x, p.y + s.y);

		setColor(color::BLUE);
		drawLine(p1, p2);
		drawLine(p1, p3);
		drawLine(p2, p4);
		drawLine(p3, p4);

		p1= math::rotatePointByAngle(p1, rotPoint, angle);
		p2= math::rotatePointByAngle(p2, rotPoint, angle);
		p3= math::rotatePointByAngle(p3, rotPoint, angle);
		p4= math::rotatePointByAngle(p4, rotPoint, angle);

		setColor(color::WHITE);
		drawLine(p1, p2);
		drawLine(p1, p3);
		drawLine(p2, p4);
		drawLine(p3, p4);
		//

		//draw Angle Lines
		setColor(color::RED);
		getText()->changeFontColor(color::RED);
		getText()->changeFontSize(12);
		drawLine(rotPoint, Point(rotPoint.x + 50, rotPoint.y));
		drawLine(rotPoint, math::rotatePointByAngle(Point(rotPoint.x + 50, rotPoint.y), rotPoint, angle));
		drawText(std::to_string(int(angle)), int(rotPoint.x + 25), int(rotPoint.y));
		//

		//Draw Position	
		setColor(color::BLUE);
		getText()->changeFontColor(color::BLUE);
		getText()->changeFontSize(12);
		int sizeP = 4;
		Rect rect(p.x - sizeP / 2, p.y - sizeP / 2, sizeP, sizeP);
		drawRect(rect);
		drawText(std::to_string((int)p.x)+","+std::to_string((int)p.y),int(p.x) ,int(p.y-15));
		//

		//animatedTexture
		
		if (IS_A(t, AnimatedTexture*)){
			getText()->changeFontSize(12);
			getText()->changeFontColor(color::BLACK);
			drawText(std::to_string(((AnimatedTexture*)t)->getCurrentFrame()),int(p.x-15), int(p.y - 10));
		}
		//
	#endif
}

void Graphics::drawText(std::string str, int x, int y) const {
	Texture* t = getText()->textToTexture(str, renderer);
	if (!t) return;

	t->setPosition({ double(x), double(y) });
	drawTexture(t);
	delete t;
}


