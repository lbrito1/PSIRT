/*
 * Trajetoria.h
 *
 *  Created on: Mar 25, 2013
 *      Author: Leo
 */

#include "Trajectory.h"
#include "../Primitives/Vector2D.h"
#include "../Primitives/Particle.h"


Trajectory* new_trajectory(Vector2D* s, Vector2D* d, int part_est) {
	Trajectory *t = malloc(sizeof(Trajectory));
	t->source=s;
	t->direction=d;
	t->n_particulas_estavel = part_est;
	t->n_particulas_atual = 0;
	return t;
}

float distance(Vector2D* p, Trajectory* t) {
	Vector2D u,*v,dist;
	v = t->direction;
	u = minus(p,t->source);

	float pp = dot_product(&u,v) / dot_product(v,v);

	Vector2D* proj = new_vector(v->x, v->y);	//TODO

	normalize_void(proj);	//TODO!
	mult_constant_void(proj,pp);
	dist = minus(&u,proj);

	double mag = magnitude(&dist);

	free(proj);

	return mag;
}


Vector2D* projection(Vector2D* p, Trajectory* t) {
	Vector2D u,*v,dist;
	v = t->direction;
	u = minus(p,t->source);

	float pp = dot_product(&u,v) / dot_product(v,v);
	Vector2D* proj = new_vector(v->x, v->y);
	normalize(proj);
	mult_constant_void(proj,pp);
	dist = minus(&u,proj);

	return proj;
}


double trajectory_force(Trajectory *t)
{
	int delta = t->n_particulas_estavel - t->n_particulas_atual;
	//	printf("\r\nATUAL = %d \t ESTAVEL = %d", t->n_particulas_atual, t->n_particulas_estavel);
	return max(0,TRAJ_FORCE_LINEAR*delta);
}

int current_status(Trajectory* t)
{
	return max(0,t->n_particulas_estavel-t->n_particulas_atual);
}



inline void directionFrom(Vector2D *point, Trajectory *t, Vector2D *direction)
{
	Vector2D begin, end;// = clone(t->source);
	set(&begin, t->source->x,t->source->y);
	set(&end, 0.0,0.0);

	sum_void(t->source, t->direction, &end);

	Vector2D u, v;
	set(&u, point->x - begin.x, point->y - begin.y);
	set(&v, end.x - begin.x, end.y - begin.y);

	float pp = ((u.x * v.x) + (u.y * v.y)) / ((v.x * v.x) + (v.y * v.y));

	Vector2D projuv;
	set(&projuv, pp * v.x, pp * v.y);

	direction->x = u.x - projuv.x;
	direction->y = u.y - projuv.y;

	//	return new_vector(u->x - projuv->x, u->y - projuv->y);
}

void update_trajectory(Trajectory *t, Particle **p, int nparticle)
{
	t->n_particulas_atual = 0;

	int i=0;
	for (i=0;i<nparticle;i++)
	{
		if (p[i]->status == ALIVE)
		{
			// Find distance from point to line segment (orthogonal)
			double distance_point_line = distance(p[i]->location,t);

			if (distance_point_line<TRAJ_PART_THRESHOLD)
			{
				t->n_particulas_atual++;
				p[i]->current_trajectories++;
			}
		}
	}
}

Vector2D* trajectory_intersection(Trajectory *t1, Trajectory *t2)
{
	Vector2D *begin_1, *end_1, *begin_2, *end_2;

	//TODO refatorar -> criar metodo getBegin, getEnd
	begin_1 = t1->source;
	end_1 = sum(t1->source, t1->direction);

	begin_2 = t2->source;
	end_2 = sum(t2->source, t2->direction);

	float ua = ((end_2->x - begin_2->x) * (begin_1->y - begin_2->y) - (end_2->y - begin_2->y) * (begin_1->x - begin_2->x)) /
			((end_2->y - begin_2->y) * (end_1->x - begin_1->x) - (end_2->x - begin_2->x) * (end_1->y - begin_1->y));

	//	float ub = ((end_1->x - begin_1->x) * (begin_1->y - begin_2->y) - (end_1->y - begin_1->y) * (begin_1->x - begin_2->x)) /
	//			((end_2->y - begin_2->y) * (end_1->x - begin_1->x) - (end_2->x - begin_2->x) * (end_1->y - begin_1->y));

	return new_vector(begin_1->x + ua * (end_1->x - begin_1->x), begin_1->y + ua * (end_1->y - begin_1->y));
}

int trajectory_stable(Trajectory *t)
{
	if (t->n_particulas_atual>=t->n_particulas_estavel) return TRUE;
	else
	{
		//		printf("\r\n %f \t %d \t %d",t->direction->x,t->n_particulas_estavel,t->n_particulas_atual);
		return FALSE;
	}
}

// Gravitacao universal
inline void resultant(Trajectory *t, Particle* p, Vector2D *resultant)
{
	directionFrom(p->location, t, resultant);
	//	normalize(ortonormal);		//TODO aparentemente nao influencia movimento das particulas

	// Find distance from point to line segment (orthogonal)
	double distance_point_line = distance(p->location,t);
	// Calculate force according to F = m1*m2/r²
	double mass_trajectory = trajectory_force(t);
	double mass_particle   = p->mass;
	double force = 0;
	if (distance_point_line>0.01)
		force = (mass_trajectory * mass_particle) / (distance_point_line*distance_point_line);
	force/=10;

	mult_constant_void(resultant,force);
}

