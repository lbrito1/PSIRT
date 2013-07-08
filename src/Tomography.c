/*
 ============================================================================
 Name        : ConfiguracaoTomografia.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <GL/glut.h>
#include <math.h>
#include "PSIRT.h"

#ifndef RES_X
#define RES_X 500
#endif

#ifndef RES_Y
#define RES_Y RES_X
#endif


PSIRT* psirt;

// ---------------------------
// *** OPENGL ***
// Desenhar configuracao (trajetorias)
// ---------------------------
void opengl_draw_configuration_lines()
{
	int i,j;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	for (i=0;i<psirt->n_projections;i++) {
		for (j=0;j<psirt->n_trajectories;j++) {
			Trajectory *t = psirt->projections[i]->lista_trajetorias[j];
			Vector2D* begin = sum(t->source,t->direction);
			Vector2D *d = clone(t->direction);
			mult_constant_void(d,-1);
			Vector2D* end = sum(t->source,d);
			free(d);

			glColor3f(1.0,1.0,1.0);
			glVertex2f( begin->x,  begin->y);
			glVertex2f( end->x,    end->y);

			free(begin);
			free(end);
		}
	}
	glEnd();
}

// ---------------------------
// *** OPENGL ***
// Desenhar particulas
// ---------------------------
void opengl_draw_particles()
{
	int i=0;
	glPointSize(7.0);
	glBegin(GL_POINTS);
	for (i = 0; i < psirt->n_particles; i++) {
		if (psirt->particles[i]->status != DEAD) {
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(psirt->particles[i]->location->x, psirt->particles[i]->location->y);
		} else {
			glColor3f(0.0, 1.0, 0.0);
			glVertex2f(psirt->particles[i]->location->x, psirt->particles[i]->location->y);
		}
	}
	glEnd();
	glFlush();  // Render now
}

void opengl_draw()
{
	opengl_draw_configuration_lines();
	opengl_draw_particles();
}
void update()
{
	run_psirt(psirt);
	glutPostRedisplay();
}

void keyboard_handler (unsigned char key, int x, int y)
{
	if (key == 27) exit(0);	//ESC = exit
}

void init_opengl(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("PSIRT");
	glutReshapeWindow(RES_X, RES_Y);
	glutDisplayFunc(opengl_draw);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard_handler);
	glutMainLoop();
}

int main(int argc,  char* argv[] )
{
	psirt = init_psirt();

#ifndef NO_GRAPHICS
	init_opengl(argc, argv);
#endif
#ifdef NO_GRAPHICS
	while(1) run_psirt(psirt);
#endif

	return 0;

}
