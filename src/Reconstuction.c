/*
 * Reconstuction.c
 *
 *  Created on: Jul 2, 2013
 *      Author: Leo
 */

#include "Reconstruction.h"





#ifndef PARTICLE_SIZE
#define PARTICLE_SIZE (int)(RES_X/10)
#endif

#ifndef RECONSTRUCTION_SCALE_FACTOR
#define RECONSTRUCTION_SCALE_FACTOR 0.4
#endif

#ifndef SATURATION
#define SATURATION 1
#endif

double* reconstruction(PSIRT* psirt)
{
	int i=0;
	//for(i=0;i<n_particulas;i++) printf("\r\nDONE\tPARTICLE #%d STATUS: %d",i,particles[i]->status);

	// ---
	// 1. AJUSTAR ESCOPO
	// O universo de particulas esta inicialmente entre -1 e +1.
	// Ajustar de acordo com o fator de escala.
	// ---
	for (i = 0; i < psirt->n_particles; i++) {
		if ((psirt->particles[i]->location->x > -RECONSTRUCTION_SCALE_FACTOR)
				&& (psirt->particles[i]->location->x < RECONSTRUCTION_SCALE_FACTOR)
				&& (psirt->particles[i]->location->y > -RECONSTRUCTION_SCALE_FACTOR)
				&& (psirt->particles[i]->location->y < RECONSTRUCTION_SCALE_FACTOR)) {
			psirt->particles[i]->location->x += RECONSTRUCTION_SCALE_FACTOR;
			psirt->particles[i]->location->y += RECONSTRUCTION_SCALE_FACTOR;
			psirt->particles[i]->location->x /= 2 * RECONSTRUCTION_SCALE_FACTOR;
			psirt->particles[i]->location->y /= 2 * RECONSTRUCTION_SCALE_FACTOR;
		}
	}
	// ---
	// 2. ESCALAR
	// Escalar particulas de acordo com resolucao.
	// ---
	Vector2D** scaled_particles = malloc(sizeof(Vector2D*) * psirt->n_particles);
	for (i = 0; i < psirt->n_particles; i++) {
		scaled_particles[i] = mult_constant(psirt->particles[i]->location, RES_X);
		//printf("\r\n ANTES: %f,%f \t DEPOIS: %f,%f",particles[i]->location->x, particles[i]->location->y,scaled_particles[i]->x,scaled_particles[i]->y  );
	}
	// ---
	// 3. CALCULAR INTENSIDADE DE CADA PIXEL
	// Para cada pixel, medir a distancia entre seu centro
	// e cada particula presente no sistema.
	// ---
	double *pixel_intensity = malloc(sizeof(double) * RES_X * RES_Y);
	for (i = 0; i < RES_X * RES_Y; i++) pixel_intensity[i] = 0.0;

	Vector2D* pixel = new_vector(0.0, 0.0);

	int pix_x = 0, pix_y = 0, part = 0;
	double iter_intensity = 0.0;
	for (pix_x = 0; pix_x < RES_X; pix_x++) {
		for (pix_y = 0; pix_y < RES_Y; pix_y++) {
			// atualiza pixel atual (centro: bias +0.5, +0.5)
			set_vector(pixel, pix_x + 0.5, pix_y + 0.5);

			// distancia para cada particula
			for (part = 0; part < psirt->n_particles; part++) {
				if (psirt->particles[part]->status != DEAD) {
					// formato: x + ( y*RES_X )
					float distance = vector_vector_distance(pixel,
							scaled_particles[part]);
					if (distance > PARTICLE_SIZE) {
						distance = 0.0;
						iter_intensity = 0.0;
					} else {
						distance = ((PARTICLE_SIZE - distance) / PARTICLE_SIZE);
//						distance= distance/PARTICLE_SIZE;
//						distance = 1-distance;
						iter_intensity = distance; //TODO interf. destrutiva?
//						iter_intensity =
//								(iter_intensity > SATURATION) ? SATURATION : iter_intensity;
					}
					if (iter_intensity > 0)
						pixel_intensity[pix_x + (pix_y * RES_X)] +=
								iter_intensity;
				}
			}
		}
	}
	free(scaled_particles);
	free(pixel);
	double max = -1.0, min = 999999.0, delta = 0;
	for (i = 0; i < RES_X * RES_Y; i++) {
		if (pixel_intensity[i] > max) max = pixel_intensity[i];
		else if (pixel_intensity[i] < min) min = pixel_intensity[i];
		//			if (i%RES_X==0) printf("\r\n");
		//			printf("%f ",pixel_intensity[i]);
	}
	delta = max - min;
	// normalize
	for (i = 0; i < RES_X * RES_Y; i++) {
		pixel_intensity[i] -= min;
		pixel_intensity[i] /= delta;
	}
	return pixel_intensity;
}

int draw_reconstruction_bitmap(PSIRT *psirt) {
	int i=0,j=0;
	double* pixel_intensity = reconstruction(psirt);
	// DESENHAR
	bmpfile_t *bmp = bmp_create(RES_X, RES_Y, 24);
	for (i = 0; i < RES_X; i++) {
		for (j = 0; j < RES_Y; j++) {
			double pixel = pixel_intensity[i + (j * RES_X)];

			// Alternativa 1: fun��o linear simples
			int paint_of_choice = (pixel * 255);

			// Alternativa 2: fun��o polinomial
			#ifdef REC_PAINT_POLY
			paint_of_choice = pow(pixel, 2) * 255;
			#endif

			// Alternativa 3: fun��o exponencial
			#ifdef REC_PAINT_EXP
			paint_of_choice = pow(pixel, pixel) * 255;
			#endif

			// Alternativa 3: fun��o customizada
			#ifdef REC_PAINT_CUSTOM
			paint_of_choice = 255;
			if (pixel < .2)
				paint_of_choice = 0;
			else if (pixel < .4)
				paint_of_choice = 255 / 4;
			#endif

			rgb_pixel_t pix = { paint_of_choice, paint_of_choice, paint_of_choice, 0 };
			bmp_set_pixel(bmp, i, j, pix);
		}
	}
	bmp_save(bmp, "psirt_output.bmp");
	bmp_destroy(bmp);
	free(pixel_intensity);
	exit(1);
	return i;
}
