#include "Particle.h"

Particle* new_particle()
{
	Particle* part = malloc(sizeof(Particle));
	Vector2D* loc = new_vector(0.0,0.0);
	Vector2D* spd = new_vector(0.0,0.0);
	part->location = loc;
	part->current_trajectories = 0;
	part->status = ALIVE;
	return part;
}

void update_particle(Particle* particle, Vector2D* resultant_force)
{
	mult_constant_void(resultant_force, PARTICLE_MASS);
	sum_void(particle->location, resultant_force, particle->location);
}
