/*
 * Vector2D.h
 *
 *  Created on: Apr 1, 2013
 *      Author: Leo
 */

//#pragma once

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

typedef struct { double x,y; } Vector2D;

void set(Vector2D* v, double x, double y);

Vector2D* sum(Vector2D* a, Vector2D* b);

void sum_void(Vector2D* a, Vector2D* b, Vector2D* result);

Vector2D* minus(Vector2D* a, Vector2D* b);

float dot_product(Vector2D* a, Vector2D* b);

double magnitude(Vector2D* a);

Vector2D* new_vector(double x, double y);

//TODO
inline Vector2D* mult_constant(Vector2D* a, double k);

void mult_constant_void(Vector2D* a, double k);

void normalize(Vector2D* a);

inline void normalize_void(Vector2D* a);

Vector2D* clone(Vector2D* a);

void copyTo(Vector2D* a, Vector2D* b);

Vector2D* RotateCounterClockWise(Vector2D* a, float angle);

Vector2D* RotateClockWise(Vector2D* a, float angle);

double vector_vector_distance(Vector2D* a, Vector2D* b);

Vector2D* lerp(Vector2D* a, Vector2D* b, double t);

double distance_point_line(Vector2D* point, Vector2D* line_a, Vector2D* line_b);

void set_vector(Vector2D* a, double x, double y);


#endif /* VECTOR2D_H_ */
