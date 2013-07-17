/*
 *
 *  Created on: Mar 24, 2013
 *      Author: Leo
 */

#include "Vector2D.h"
#include <math.h>
#define PI 3.1415926536
#define DEGREE_TO_RAD PI/180.0
#define RAD_TO_DEGREE 180.0/PI

void set_vector(Vector2D* a, double x, double y)
{
	a->x=x;a->y=y;
}

Vector2D* sum(Vector2D* a, Vector2D* b)
{
	Vector2D* c = malloc(sizeof(Vector2D));
	c->x = a->x+b->x;
	c->y = a->y+b->y;
	return c;
}

void sum_void(Vector2D* a, Vector2D* b, Vector2D* result)
{
	result->x = a->x+b->x;
	result->y = a->y+b->y;
}

Vector2D minus(Vector2D* a, Vector2D* b)
{
	Vector2D c;
	c.x = a->x-b->x;
	c.y = a->y-b->y;
	return c;
}

void minus_void(Vector2D* a, Vector2D* b, Vector2D* c)
{
	c->x = a->x-b->x;
	c->y = a->y-b->y;
}

float dot_product(Vector2D* a, Vector2D* b)
{
	return (a->x*b->x + a->y*b->y);
}


double magnitude(Vector2D* a)
{
	return sqrt((a->x * a->x)+(a->y * a->y));
}


Vector2D* new_vector(double x, double y) {
	Vector2D* a = malloc(sizeof(Vector2D));
	a->x=x;
	a->y=y;
	return a;
}

Vector2D* mult_constant(Vector2D* a, double k)
{
	return new_vector((a->x)*k,(a->y)*k);
}


void mult_constant_void(Vector2D* a, double k)
{
	a->x=a->x*k;
	a->y=a->y*k;
}


inline void normalize(Vector2D* a)
{
	a = mult_constant(a,1/magnitude(a));
}

inline void normalize_void(Vector2D* a)
{
	double constant = 1/magnitude(a);
	mult_constant_void(a,constant);
}
Vector2D* clone(Vector2D* a)
{
	Vector2D* cloned = malloc(sizeof(Vector2D));
	cloned->x = a->x;
	cloned->y = a->y;
	return cloned;
}

void copyTo(Vector2D* a, Vector2D* b)
{
	b->x = a->x;
	b->y = a->y;
}

void RotateCounterClockWise(Vector2D* a, float angle)
{
    float angleRad = angle *DEGREE_TO_RAD;
    double x = a->x * cos(angleRad) - a->y * sin(angleRad);
    double y = a->x * sin(angleRad) + a->y *cos(angleRad);
    a->x = x;
    a->y = y;
}

void set(Vector2D* v, double x, double y)
{
	v->x=x;
	v->y=y;
}

void RotateClockWise(Vector2D* a, float angle)
{
	RotateCounterClockWise(a, -angle);
}


double vector_vector_distance(Vector2D* a, Vector2D* b)
{
  Vector2D c = minus(a,b);
  double d = magnitude(&c);
  return d;
}


void lerp(Vector2D* a, Vector2D* b, double t, Vector2D* c)
{
	c->x = a->x * t + (b->x * (1-t));
	c->y = a->y * t + (b->y * (1-t));
}

double distance_point_line(Vector2D* point, Vector2D* line_a, Vector2D* line_b)
{
	double x0,y0,x1,y1,x2,y2;
	x0 = point->x; y0 = point->y;
	x1 = line_a->x; y1 = line_a->y;
	x2 = line_b->x; y2 = line_b->y;

	return ((fabs(((x2-x1)*(y1-y0))-((x1-x0)*(y2-y1)))) / sqrt(pow((x2-x1),2))+pow((y2-y1),2));
}
