/*
 * ProjectionController.c
 *
 *  Created on: Apr 1, 2013
 *      Author: Leo
 */

#include "ProjectionController.h"

Projection** new_parallel_projections(int n_projections, int n_trajectories, int** particles_per_trajectory)
{
	Projection** projections = malloc(sizeof(Projection)*n_projections);
	float angle_step = 180.0/(float)n_projections;
	float angle = 0;
	int i=0;

	for (i=0;i<n_projections;i++,angle+=angle_step)
	{
		projections[i] = new_parallel_projection(angle, n_trajectories, particles_per_trajectory[i]);
	}

	return projections;
}

int** get_dummy_particle_per_projection_trajectory(int n_proj, int n_traj, int n_part)
{
	int **part_per_traj = malloc(sizeof(int)*n_proj);
	int i,j;

	for (i=0;i<n_proj;i++) //init
		part_per_traj[i] = malloc(sizeof(int)*n_traj);

	for (i=0;i<n_proj;i++)
		for (j=0;j<n_traj;j++)
			part_per_traj[i][j]=n_part;

	return part_per_traj;
}
