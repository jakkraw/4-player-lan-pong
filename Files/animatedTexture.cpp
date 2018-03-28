#include "animatedTexture.h"



AnimatedTexture::AnimatedTexture(): Texture(){
	frameSize = { 0,0,0,0 };
	loopAnimation = true;
	animationEnded = false;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	animationTimer = 0;
	frameDelay = 1.0;
	columns = 1;
}

AnimatedTexture::AnimatedTexture(const AnimatedTexture & other):Texture(other){
	frameSize = other.frameSize;
	loopAnimation = other.loopAnimation;
	animationEnded = other.animationEnded;
	startFrame = other.startFrame;
	endFrame = other.endFrame;
	currentFrame = other.currentFrame;
	animationTimer = other.animationTimer;
	frameDelay = other.frameDelay;
	columns = other.columns;
}

AnimatedTexture & AnimatedTexture::operator=(const AnimatedTexture & other)
{
	Texture::operator=(other);
	frameSize = other.frameSize;
	loopAnimation = other.loopAnimation;
	animationEnded = other.animationEnded;
	startFrame = other.startFrame;
	endFrame = other.endFrame;
	currentFrame = other.currentFrame;
	animationTimer = other.animationTimer;
	frameDelay = other.frameDelay;
	columns = other.columns;
	return *this;
}


void AnimatedTexture::changeFrame(int currentFrame){
		frameSize.x =(currentFrame % columns) * frameSize.w ;
		frameSize.y =(currentFrame / columns) * frameSize.h;
		source = frameSize;
}


void AnimatedTexture::update(double frameTime)
{
	if (endFrame - startFrame != 0)          
	{
		animationTimer += frameTime;             
		if (animationTimer > frameDelay)
		{
			animationTimer -= frameDelay;

			(endFrame - startFrame) > 0 ? currentFrame++ : currentFrame--;

			if (currentFrame < startFrame || currentFrame > endFrame)			
			{
				if (loopAnimation == true)         
					currentFrame = startFrame;
				else                        
				{
					currentFrame = endFrame;
					animationEnded = true;
				}
			}
			changeFrame(currentFrame);
		}
	}
}

void AnimatedTexture::setSize(Vector p){
	Texture::setSize(p);
}


int AnimatedTexture::getStartFrame() const{
	return startFrame;
}

int AnimatedTexture::getEndFrame() const{
	return endFrame;
}

int AnimatedTexture::getCurrentFrame() const{
	return currentFrame;
}


void AnimatedTexture::setCurrentFrame(int frame){
	this->currentFrame = frame;
	changeFrame(currentFrame);
}

void AnimatedTexture::setColumns(int columns) {
	this->columns = columns;
}

void AnimatedTexture::setFrameDelay(double delay){
	this->frameDelay = delay;
}

void AnimatedTexture::setSourceFrameSize(double width, double height) {
	setSourceFrameSize({ width,height });
}

void AnimatedTexture::setSourceFrameSize(Vector p){
	frameSize = { frameSize.x,frameSize.y, p.x,p.y };
}

void AnimatedTexture::setFrames(int from, int to){
	this->startFrame = from;
	this->endFrame = to;
}

void AnimatedTexture::setStartFrame(int frame){
	this->startFrame = frame;
}

void AnimatedTexture::setEndFrame(int frame) {
	this->endFrame = frame;
}

