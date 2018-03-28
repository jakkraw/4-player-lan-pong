#pragma once
#include <SDL.h>
struct Point;
typedef Point Vector;

struct Point
{
	double x, y;
	Point() :x(0), y(0) {};
	Point(double x, double y) : x(x), y(y) {};

	operator SDL_Point() const { return{ int(x),int(y) }; }
	Point operator+(const Point&p) const { return Point(this->x + p.x, this->y + p.y); }
	Point operator-(const Point&p) const { return Point(this->x - p.x, this->y - p.y); }
	Point operator*(const Point&p) const { return Point(this->x * p.x, this->y * p.y); }
	Point operator/(const Point&p) const { return Point(this->x / p.x, this->y / p.y); }
	Point operator+(const double&p) const { return Point(this->x + p, this->y + p); }
	Point operator-(const double&p) const { return Point(this->x - p, this->y - p); }
	Point operator*(const double&p) const { return Point(this->x * p, this->y * p); }
	Point operator/(const double&p) const { return Point(this->x / p, this->y / p); }
	bool operator == (const Point&p) const {return ((this->x == p.x) && (this->y == p.y));}
	bool operator != (const Point&p) const {return !operator==(p);}

	double length() {
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	Point getUnitVector() {
		return *this / this->length();
	}

};