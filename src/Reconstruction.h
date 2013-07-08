/*
 * Reconstruction.h
 *
 *  Created on: Jul 2, 2013
 *      Author: Leo
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Primitives/Particle.h"
#include "PSIRT.h"
#include "libs/bmpfile.h"

#ifndef RECONSTRUCTION_H_
#define RECONSTRUCTION_H_

#ifndef RES_X
#define RES_X 500
#endif

#ifndef RES_Y
#define RES_Y RES_X
#endif

double* reconstruction(PSIRT* psirt);
int draw_reconstruction_bitmap(PSIRT *psirt);


#endif /* RECONSTRUCTION_H_ */
