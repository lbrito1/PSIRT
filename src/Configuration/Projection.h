/*
 * Projection.h
 *
 *  Created on: Apr 1, 2013
 *      Author: Leo
 */

#pragma once

#ifndef PROJECTION_H_
#define PROJECTION_H_

#include "Trajectory.h"

typedef struct {
	Trajectory** lista_trajetorias;
	int n_traj;
} Projection;

void init_proj(Projection* p, double angle, int ntraj, int* partptraj);

int projection_stable(Projection *p);

int has_converged(Projection **p, int n_projections);

#endif /* PROJECTION_H_ */
