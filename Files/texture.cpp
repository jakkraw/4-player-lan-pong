#include "texture.h"


Texture::Texture(){
	strcpy(name, "none");
	this->debug = true;
	this->visible = true;
	this->initialized = false;
	this->texture = nullptr;
}

Texture::Texture(const Texture & other):Texture(){
	this->debug = other.debug;
	this->visible = other.visible;
	this->angle = other.angle;
	this->flip = other.flip;
	this->rotationCenter = other.rotationCenter;
	this->source = other.source;
	this->info = other.info;
	strcpy(name, other.name);

}

Texture::~Texture() {	
		SDL_DestroyTexture(texture);		
}

Texture & Texture::operator=(const Texture & other){
	this->debug = other.debug;
	this->visible = other.visible;
	this->angle = other.angle;
	this->flip = other.flip;
	this->rotationCenter = other.rotationCenter;
	this->info = other.info;
	strcpy(name, other.name);
	return *this;
}


bool Texture::initialize(SDL_Surface * surface, SDL_Renderer* renderer)
{
	SDL_DestroyTexture(texture);
	texture = 0;
	this->texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture){
		LogError("Unable to create texture from surface!  %s\n",SDL_GetError());
		return false;
	}

	setSource({ 0.0,0.0,double(surface->w),double(surface->h) });
	initialized =true;
	return true;
}

void Texture::setPosition(Point p){
		info.x = p.x ;
		info.y = p.y ;
}

void Texture::setRotCenter(Point p) {
	rotationCenter = p;
}

void Texture::setVisible(bool visible){
	this->visible = visible;
}

void Texture::setFlip(SDL_RendererFlip flip){
	this->flip = flip;
}

void Texture::setName(std::string name){
	if(!name.empty())
		strcpy(this->name,name.c_str());
}

void Texture::setSource(Rect source) {
	this->source = source;
}

void Texture::setInfo(Rect size) {
	this->info = size;
}

void Texture::setSize(Vector p)
{
	this->info.w = p.x;
	this->info.h = p.y;
}

void Texture::setAngle(double x) {
	angle = x;
	angle = remainder(angle, 360);
	if (angle < 0) {
		angle += 360;
	}	
}


Point Texture::getRotCenter() const{
	return rotationCenter;
}

Rect Texture::getInfo() const{
	 return info;
}

Point Texture::getPosition() const{
	 return info.pos();
}

Vector Texture::getSize() const {
	return info.size();
}

Rect Texture::getSource() const{
	return source;
}


SDL_Texture * Texture::getSDLTexture() const{
	return texture;
}

SDL_RendererFlip Texture::getFlip() const{
	return flip;
}

double Texture::getAngle() const{
	return angle;
}

bool Texture::getVisible() const{
	return visible;
}

bool Texture::isInitialized() const{
	return initialized;
}

std::string Texture::getName() const{
	return name;
}
