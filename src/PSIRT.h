/*
 * PSIRT.h
 *
 *  Created on: Jul 2, 2013
 *      Author: Leo
 */
#pragma once
#ifndef PSIRT_H_
#define PSIRT_H_

#define DEAD 	 0
#define ALIVE 	 1
#define CHECKING 2

#define OPTIM_MAX_ITER 100

#include "Configuration/ProjectionController.h"
#include "Primitives/Particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


typedef struct {
	Projection** projections;
	Particle** particles;

	int n_projections;
	int n_trajectories;
	int n_particles;

	int is_optimizing_dirty_particle;
	int optim_is_ranked;

	int optim_curr_part;
	int optim_curr_iteration;
	int optim_max_iterations;
} PSIRT;

PSIRT* init_psirt();
int update_particles(PSIRT* psirt);
void optimize(PSIRT* psirt);
void run_psirt(PSIRT* psirt);
void optimization_check(PSIRT* psirt);
void init_particles(PSIRT* psirt);
void read_sinogram(PSIRT* psirt);

#endif /* PSIRT_H_ */
