#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "types/geometry.h"

#define MIN_X_COORD		350 //420
#define MAX_X_COORD		3890
#define MIN_Y_COORD 	230  //270
#define MAX_Y_COORD 	3880 //3820

#define X_SPAN 			(MAX_X_COORD - MIN_X_COORD)
#define Y_SPAN 			(MAX_Y_COORD - MIN_Y_COORD)

#define N_BOX_X 	 	21
#define N_BOX_Y 		27

#define UNIT_X_COORD	(X_SPAN / N_BOX_X)
#define UNIT_Y_COORD 	(Y_SPAN / N_BOX_Y)

typedef struct Box {
	int16_t i;
	int16_t j;
} Box;

extern const Box target_box;

bool Field_IsEqualBox(Box b1, Box b2);
Coordinate Field_GetMiddleCoordOfBox(Box b);
Box Field_GetBoxFromCoord(Coordinate c);
Box Field_GetBoxFromDelta(Box cur, int16_t delta_i, int16_t delta_j);

#endif
