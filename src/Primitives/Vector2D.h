/*
 * Vector2D.h
 *
 *  Created on: Apr 1, 2013
 *      Author: Leo
 */

#pragma once

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

typedef struct { float x,y; } Vector2D;

void set(Vector2D* v, float x, float y);

Vector2D* sum(Vector2D* a, Vector2D* b);

void sum_void(Vector2D* a, Vector2D* b, Vector2D* result);

Vector2D minus(Vector2D* a, Vector2D* b);

void minus_void(Vector2D* a, Vector2D* b, Vector2D* c);

float dot_product(Vector2D* a, Vector2D* b);

float magnitude(Vector2D* a);

Vector2D* new_vector(float x, float y);

//TODO
inline Vector2D* mult_constant(Vector2D* a, float k);

void mult_constant_void(Vector2D* a, float k);

void normalize(Vector2D* a);

inline void normalize_void(Vector2D* a);

Vector2D* clone(Vector2D* a);

void copyTo(Vector2D* a, Vector2D* b);

void RotateCounterClockWise(Vector2D* a, float angle);

void RotateClockWise(Vector2D* a, float angle);

float vector_vector_distance(Vector2D* a, Vector2D* b);

void lerp(Vector2D* a, Vector2D* b, float t, Vector2D* c);

float distance_point_line(Vector2D* point, Vector2D* line_a, Vector2D* line_b);

void set_vector(Vector2D* a, float x, float y);


#endif /* VECTOR2D_H_ */
