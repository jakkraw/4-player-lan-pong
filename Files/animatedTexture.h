#ifndef ANIMATED_TEXTURE_H
#define ANIMATED_TEXTURE_H

#include "texture.h"

class AnimatedTexture : public Texture
{
private:
	Rect frameSize;
	bool loopAnimation;
	bool animationEnded;
	int startFrame;
	int endFrame;
	int currentFrame;
	double animationTimer;
	double frameDelay;
	int columns;

	void changeFrame(int currentFrame);
	
public:
	
	AnimatedTexture();
	AnimatedTexture(const AnimatedTexture& other);
	AnimatedTexture& operator=(const AnimatedTexture& other);

	void update(double frameTime) override;

	void setSize(Vector p);
	void setSourceFrameSize(double width, double height);
	void setSourceFrameSize(Vector p);
	void setStartFrame(int frame);
	void setEndFrame(int frame);
	void setCurrentFrame(int frame);
	void setColumns(int columns);
	void setFrameDelay(double delay);
	
	void setFrames(int from, int to);

	int getStartFrame() const;	
	int getEndFrame() const;
	int getCurrentFrame() const;
	
};

#endif










