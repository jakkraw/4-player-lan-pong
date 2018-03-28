#pragma once
#include "point.h"
#include "SDL.h"

struct Rect
{
	double x, y, w, h;
	Rect() :x(0), y(0), w(0), h(0) {};
	Rect(double x, double y, double w, double h) : x(x), y(y), w(w), h(h) {};
	Rect(Point a, Vector b) :x(a.x), y(a.y), w(b.x), h(b.y) {};
	Point pos() const { return{ x,y }; }
	Point size() const { return{ w,h }; }
	operator SDL_Rect() const { return{ int(x),int(y),int(w),int(h) }; }
};