/*
 * Particle.h
 *
 *  Created on: Mar 25, 2013
 *      Author: Leo
 */

#pragma once

#ifndef PARTICLE_H_
#define PARTICLE_H_

#define DEAD 	 0
#define ALIVE 	 1
#define CHECKING 2

#define PARTICLE_MASS 0.01

#include "Vector2D.h"

typedef struct {
	Vector2D* location;
	int current_trajectories;
	int status;
} Particle;

Particle* new_particle();

void update_particle(Particle* particle, Vector2D* resultant_force);

#endif /* PARTICLE_H_ */
