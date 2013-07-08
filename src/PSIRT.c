/*
 * PSIRT.c
 *
 *  Created on: Jul 2, 2013
 *      Author: Leo
 */

#include "PSIRT.h"

struct timeval tv;
time_t curtime, timestep;

PSIRT* init_psirt()
{
	gettimeofday(&tv, NULL);
	curtime=tv.tv_usec;

	PSIRT *psirt;
	psirt = malloc(sizeof(PSIRT));
	psirt->is_optimized = 0;
	psirt->is_optimizing_dirty_particle = 0;
	psirt->optim_is_ranked = 0;
	psirt->optim_curr_part = 0;
	psirt->optim_curr_iteration = 0;
	psirt->optim_max_iterations = 100;

	read_sinogram(psirt);
	init_particles(psirt);

	printf("\r\n[DONE] \t INIT \t %d us",(timestep=(tv.tv_usec-curtime)));

	return psirt;
}

void run_psirt(PSIRT* psirt)
{
	// ---------------------------
	// *** ATUALIZAR POSICOES DAS PARTICULAS ***
	// ---------------------------
	update_particles(psirt);

	// ---------------------------
	// *** CALCULO DE TRAJETORIAS SATISFEITAS ***
	// ---------------------------
	int i=0,j=0;
	for (i=0;i<psirt->n_particles;i++) psirt->particles[i]->current_trajectories = 0; 	// zera #traj de cada particula
	for (i=0;i<psirt->n_projections;i++) 											// calcula #traj de cada particula
		for (j=0;j<psirt->n_trajectories;j++)
			update_trajectory(psirt->projections[i]->lista_trajetorias[j], psirt->particles, psirt->n_particles);

	// ---------------------------
	// *** OTIMIZACAO E CONVERGENCIA ***
	// ---------------------------
	optimization_check(psirt);													// otimizacao (continuar)
	if (has_converged(psirt->projections,psirt->n_projections))							// convergiu
	{
		if (psirt->optim_curr_part < psirt->n_particles) optimize(psirt);						// otimizacao (comecar)
		else
		{
#ifndef NO_RECON
//			printf("\r\n[DONE] \t OPTIMZ (CONVERGED) \t %d us",(timestep = (tv.tv_usec-timestep)));
			draw_reconstruction_bitmap(psirt);									// terminou otimizacao: desenhar
//			printf("\r\n[DONE] \t RECONSTRUCTION \t %d us\r\n",(timestep = (tv.tv_usec-timestep)));
#endif
#ifdef NO_RECON
//			printf("\r\n[DONE] \t OPTIMZ (CONVERGED) \t %d us\r\n",((tv.tv_usec-timestep)));
			exit(1);
#endif
		}
	}
}

void optimize(PSIRT* psirt)
{
	int i = 0, j = 0;

	// ---------------------------
	// *** PRIMEIRO PASSO ***
	// Deve-se ordenar as particulas de acordo
	// com a quantidade de trajetorias a que
	// cada uma atende (0 a 3), ordem crescente.
	// ---------------------------
	if (!psirt->optim_is_ranked)
	{
		Particle* temp = new_particle();
		for (i = 0; i < psirt->n_particles; i++)
		{
			for (j = 0; j < psirt->n_particles; j++)
			{
				if (psirt->particles[j]->current_trajectories
						> psirt->particles[i]->current_trajectories) {
					memcpy(temp, psirt->particles[j], sizeof(Particle));
					memcpy(psirt->particles[j], psirt->particles[i], sizeof(Particle));
					memcpy(psirt->particles[i], temp, sizeof(Particle));
				}
			}
		}
		free(temp);
		psirt->optim_is_ranked = 1;
	}

	// ---------------------------
	// CASO ESPECIAL
	// partícula sem trajetoria, ELIMINAR SEM CHECAR
	// ---------------------------
	if (psirt->particles[psirt->optim_curr_part]->current_trajectories == 0)
	{
		//printf("\r\n[OPTIM]\tPARTICLE #%d DIED (0 TRAJ)", optim_curr_part);
		psirt->is_optimizing_dirty_particle = 0;
		psirt->particles[psirt->optim_curr_part]->status = DEAD;
		psirt->optim_curr_part++;
	}

	// ---------------------------
	// CASO NORMAL
	// partícula atende a mais de 0 trajetórias, CHECAR ANTES
	// ---------------------------
	else
	{
		psirt->is_optimizing_dirty_particle = 1;

		// COMECAR A CHECAR PARTICULA
		if (psirt->particles[psirt->optim_curr_part]->status == ALIVE)
		{
			psirt->optim_curr_iteration = 0;
			psirt->particles[psirt->optim_curr_part]->status = CHECKING;
		}
		// PARTICULA CHECADA & CONVERGIU -> REMOVER
		else if (psirt->particles[psirt->optim_curr_part]->status == CHECKING)
		{
			//printf("\r\n[OPTIM]\tPARTICLE #%d DIED (ITER #%d)", optim_curr_part,optim_curr_iteration);
			psirt->particles[psirt->optim_curr_part]->status = DEAD;
			psirt->optim_curr_part++;
		}
	}
}


void optimization_check(PSIRT* psirt)
{
	// pre-update otimizacao
	if (psirt->is_optimizing_dirty_particle) {
		// OTIMIZANDO
		if (psirt->optim_curr_iteration < psirt->optim_max_iterations) {
			psirt->optim_curr_iteration++;
		}
		// OTIMIZACAO FALHOU (EXCEDEU MAX ITERACOES)
		else {
			//			printf("\r\n[OPTIM]\tPARTICLE #%d LIVED (ITER #%d)",optim_curr_part,optim_curr_iteration);
			psirt->particles[psirt->optim_curr_part]->status = ALIVE; // NAO CONSEGUIU REMOVER
			psirt->optim_curr_part++;
			psirt->is_optimizing_dirty_particle = 0;
		}
	}
}

int update_particles(PSIRT* psirt)
{
	int i=0,j=0,k=0;
	Vector2D* resultant_force = new_vector(0, 0);
	for (i = 0; i < psirt->n_particles; i++) {
		if (psirt->particles[i]->status != DEAD) {

			resultant_force->x = 0.0;
			resultant_force->y = 0.0;
			// calcular força resultante primeiro
			Vector2D* resultant_vector = new_vector(0.0, 0.0);
			for (j = 0; j < psirt->n_projections; j++) {
				for (k = 0; k < psirt->n_trajectories; k++) {
					resultant(psirt->projections[j]->lista_trajetorias[k],
							psirt->particles[i], resultant_vector);
					sum_void(resultant_force, resultant_vector,
							resultant_force);
				}

			}
			free(resultant_vector);

			resultant_force->x = -resultant_force->x;
			resultant_force->y = -resultant_force->y;
			update_particle(psirt->particles[i], resultant_force);

		}
	}
	free(resultant_force);
	return i;
}

// ---------------------------
// *** INICIALIZAR PARTICULAS ***
// ---------------------------
void init_particles(PSIRT* psirt) {
	srand(time(NULL ));
	int i = 0;
	int lim = RAND_MAX / 2;
	psirt->particles = malloc(sizeof(Particle) * psirt->n_particles);
	for (i = 0; i < psirt->n_particles; i++) {
		psirt->particles[i] = new_particle();
		psirt->particles[i]->location->x = rand() / (double) RAND_MAX;
		psirt->particles[i]->location->y = rand() / (double) RAND_MAX;
		if (rand() > lim)
			psirt->particles[i]->location->x = -psirt->particles[i]->location->x;
		if (rand() > lim)
			psirt->particles[i]->location->y = -psirt->particles[i]->location->y;
	}

}


