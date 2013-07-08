/*
 * ProjectionController.h
 *
 *  Created on: Apr 1, 2013
 *      Author: Leo
 */

#pragma once
#include <stdlib.h>
#include <stdio.h>
#ifndef PROJECTIONCONTROLLER_H_
#define PROJECTIONCONTROLLER_H_

#include "Projection.h"

Projection** new_parallel_projections(int n_projections, int n_trajectories, int** particles_per_trajectory);

int** get_dummy_particle_per_projection_trajectory(int n_proj, int n_traj, int n_part);

#endif /* PROJECTIONCONTROLLER_H_ */
