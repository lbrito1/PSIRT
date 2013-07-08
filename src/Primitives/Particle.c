#include "Particle.h"

double particle_mass = 0.01;

Particle* new_particle()
{
	Particle* part = malloc(sizeof(Particle));
	Vector2D* loc = new_vector(0.0,0.0);
	Vector2D* spd = new_vector(0.0,0.0);
	Vector2D* acc = new_vector(0.0,0.0);
	part->location = loc;
	part->speed = spd;
	part->acceleration = acc;
	part->mass = particle_mass;
	part->current_trajectories = 0;
	part->status = ALIVE;
	return part;
}

void update_particle(Particle* particle, Vector2D* resultant_force)
{
	// actual acceleration delta
	if (particle->mass<.00001)
	{
		resultant_force->x=0;
		resultant_force->y=0;
	}
	else
	{
		mult_constant_void(resultant_force, particle_mass);	// TODO gambiarra física
	}
	// update acceleration with new delta
	particle->acceleration->x = resultant_force->x;
	particle->acceleration->y = resultant_force->y;

	particle->speed->x=0.0;
	particle->speed->y=0.0;

	// update speed by summing curent speed vector with acceleration vector
	sum_void(particle->speed, particle->acceleration, particle->speed);

	// update location
	sum_void(particle->location, particle->speed, particle->location);
}
