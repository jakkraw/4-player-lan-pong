#pragma once

#include "point.h"
#include "math.h"


struct Line
{
	bool higher;
	double a;
	double b;
	Point p1;
	Point p2;
	Line() { a = 0; b = 0; p1 = Point(0, 0); p2 = Point(0, 0); higher = false; };
	Line(Point pA, Point pB)
	{
		p1 = pA;
		p2 = pB;

		//podejrzane do naprawy

		if (pA.x - pB.x == 0) {
			this->a = -INFINITY;
			this->b = pA.y - this->a* pA.x;
		}
		else {
			this->a = (pA.y - pB.y) / (pA.x - pB.x);
			this->b = pA.y - this->a* pA.x;
		}

		if (a > INFINITY) a = INFINITY;
		if (a < -INFINITY) a = -INFINITY;
	}

	Line(double a, double b)
	{
		if (a > INFINITY) a = INFINITY;
		if (a < -INFINITY) a = -INFINITY;
		this->a = a;
		this->b = b;
	}

	double getY(double x) const {
		return a*x + b;
	}

	bool isPointBelow(Point* p) const
	{

		if (a <= -INFINITY) {
			if (p->x < p1.x)
				return true;
			else return false;
		}

		if (this->getY(p->x) < p->y) return true;
		else return false;
	}

	bool isPointAbove(Point* p) const
	{
		if (a <= -INFINITY) {
			if (p->x > p1.x)
				return true;
			else return false;
		}

		if (this->getY(p->x) > p->y) return true;
		else return false;
	}
};
