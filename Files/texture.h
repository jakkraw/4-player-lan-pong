#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL_image.h"
#include "math.h"
#include "logger.h"
#include "debug.h"


class Texture
{
private:
	bool visible;
	bool initialized;
	char name[30];
	
protected:

	double angle;
	SDL_Texture* texture;
	SDL_RendererFlip flip;
	Point rotationCenter;
	Rect source;
	Rect info;


public:
	bool debug;

	Texture();
	Texture(const Texture&);
	virtual ~Texture();
	Texture& operator=(const Texture&);
	
	bool initialize(SDL_Surface*, SDL_Renderer*);
	bool isInitialized()const;
	
	Rect getSource() const;
	Rect getInfo() const;
	Point getRotCenter() const;
	Point getPosition() const;
	Vector getSize() const;
	SDL_Texture* getSDLTexture() const;
	SDL_RendererFlip getFlip() const;
	double getAngle() const;
	bool getVisible() const;
	std::string getName() const;
	

	void setVisible(bool);
	void setPosition(Point);
	void setRotCenter(Point);

	void setInfo(Rect);
	void setSize(Vector);
	void setSource(Rect);
	void setAngle(double);
	void setFlip(SDL_RendererFlip);
	void setName(std::string);
	


	virtual void update(double) {};

};

#endif









