#ifndef MATH_H
#define MATH_H

#include <math.h>
#undef INFINITY
#define INFINITY 10000.0

#ifndef M_PI 
#define M_PI 3.14159265358979323846 
#endif

typedef double rad;
typedef double deg;

#include "point.h"
#include "line.h"
#include "rect.h"

namespace math{

	inline double distanceFromPointToLine(Point p, Line l) {
		return fabs(l.a*p.x + -p.y + l.b) / (sqrt(l.a*l.a + 1));
	}

	inline Line fromAandPoint(double a, Point p)
	{
		if (a > INFINITY) a = INFINITY;
		if (a < -INFINITY) a = -INFINITY;

		Line l;
		l.a = a;
		l.p1 = p;

		l.b = p.y - a*p.x;
		return l;
	}

	inline Line lineFromVectorAndPoint(Vector v, Point p)
	{
		Line l;
		if (v.x == 0) {
			l.a = -INFINITY;
		}
		else {
			l.a = tan(v.y / v.x);
			if (l.a > INFINITY)l.a = INFINITY;
			if (l.a < -INFINITY) l.a = -INFINITY;
		}

		l.b = v.y - l.a *v.x;
		l.p1 = p;

		return l;
	}

	inline Point intersectPoint(Line a, Line b)
	{
		Point p;
		if (b.a - a.a == 0) return{ 0,0 };

		if (a.a <= -INFINITY)
		{
			p.x = a.p1.x;
			p.y = b.a*p.x + b.b;
			return p;
		}

		if (b.a <= -INFINITY)
		{
			p.x = b.p1.x;
			p.y = a.a*p.x + a.b;
			return p;
		}

		p.x = (a.b - b.b) / (b.a - a.a);
		p.y = a.a*p.x + a.b;
		return p;
	}

	inline Point vectorFromAngleAndLength(rad radians, double length){
		return{ cos(radians)* length,sin(radians)* length };
	}

	inline double distance(Point a, Point b) {
		return sqrt(pow((a - b).x,2) + pow((a - b).y,2));
	}


	inline Point rotatePointByAngle(Point a, Point rotationPoint, deg degree)
	{
		rad radians = degree*M_PI / 180;

		Point p;
		p.x = cos(radians) * (a.x - rotationPoint.x) - sin(radians) * (a.y - rotationPoint.y) + rotationPoint.x;
		p.y = sin(radians) * (a.x - rotationPoint.x) + cos(radians) * (a.y - rotationPoint.y) + rotationPoint.y;

		return p;
	}


	inline bool isValueBetween(double a, double b1, double b2){
		if ((a <= b1 && a>=b2) || (a >= b1 && a <= b2))
			return true;
		
		return false;
	}


	inline Line createLineNormal(Line l,Point p)
	{
		if (fabs(l.a) >= INFINITY){
			return math::fromAandPoint(0, p);		
		}
		else if (l.a != 0) {
			return math::fromAandPoint(-1.0 / l.a, p);		
		}
		else {
			return  math::fromAandPoint(-INFINITY, p);	
		}
	}


	inline bool checkIntersections(Line line, Point pos, double r)
	{

		//jesli jest pionowa linia
		if (line.a <= -INFINITY) {
			if (math::isValueBetween(line.p1.x, pos.x + r, pos.x - r)) {
				if (math::isValueBetween(pos.y - r, line.p1.y, line.p2.y) ||
					math::isValueBetween(pos.y + r, line.p1.y, line.p2.y)) {
					return true;
				}
				else return false;
			}
			else return false;
		}

		double a = (line.a*line.a) + 1;
		double b = 2 * ((line.a*(line.b - pos.y)) - pos.x);
		double c = (pos.x*pos.x) + ((line.b - pos.y)*(line.b - pos.y)) - (r*r);
		double delta = (b*b) - (4 * a*c);

		if (delta < 0) {
			return false;
		}
		else if (delta == 0) {
			if (math::isValueBetween(((-1)*b) / (2 * a), line.p1.x, line.p2.x)) {
				return true;
			}
		}
		else {

			if (math::isValueBetween((((-1)*b) - sqrt(delta)) / (2 * a), line.p1.x, line.p2.x)) {
				return true;
			}

			if (math::isValueBetween((((-1)*b) + sqrt(delta)) / (2 * a), line.p1.x, line.p2.x)) {
				return true;
			}
		}

		return false;

	}


	inline void markHigherLine(Line* a, Line*b)
	{
		if (a->a <= -INFINITY && b->a <= -INFINITY)
		{
			if (a->p1.x >= b->p1.x) {
				a->higher = true;
				b->higher = false;
			}
			else {
				a->higher = false;
				b->higher = true;
			}
		}
		else {
			if (a->b <= b->b) {
				a->higher = true;
				b->higher = false;
			}
			else {
				a->higher = false;
				b->higher = true;
			}
		}
	}


	//lines 0,2 i 1,3 równoleg³e
	inline bool pointInsideLines(Line lines[], Point* p){
		math::markHigherLine(&lines[0], &lines[2]);
		math::markHigherLine(&lines[1], &lines[3]);

		// jesli srodek kolka w srodku recta
		bool isInside = true;
		for (int i = 0; i < 4; i++){
			if (lines[i].higher == true){
				isInside = lines[i].isPointBelow(p);
			}
			else{
				isInside = lines[i].isPointAbove(p);
			}

			if (isInside == false) {
				return false;
			}
		}

		return true;
	}

	inline void rectanglePointsLines(Point points[], Line lines[], Point aHPos, Vector aHSize, Point aCenter, deg aAngle){

		points[0] = math::rotatePointByAngle({ aHPos.x, aHPos.y }, aCenter, aAngle);
		points[1] = math::rotatePointByAngle({ aHPos.x + aHSize.x, aHPos.y }, aCenter, aAngle);
		points[2] = math::rotatePointByAngle({ aHPos.x + aHSize.x, aHPos.y + aHSize.y }, aCenter, aAngle);
		points[3] = math::rotatePointByAngle({ aHPos.x, aHPos.y + aHSize.y }, aCenter, aAngle);

		lines[0] = Line(points[0], points[1]);
		lines[1] = Line(points[1], points[2]);
		lines[2] = Line(points[2], points[3]);
		lines[3] = Line(points[3], points[0]);

	}

}


#endif