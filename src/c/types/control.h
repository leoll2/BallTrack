/*
 * control.h
 *
 *  Created on: 10/feb/2019
 *      Author: leona
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdlib.h>
#include "geometry.h"

typedef Coordinate Position;


/* Controller actuation */
typedef struct Actuation {
	int16_t x;
	int16_t y;
} Actuation;


/* Position error */
typedef struct Error {
	int16_t x;
	int16_t y;
} Error;


/* Cumulative position error */
typedef struct ErrorSum {
	int32_t x;
	int32_t y;
} ErrorSum;


/* Controller state */
typedef struct ControllerState {
	Position pos_prev1;		// position 1 sample ago
	Actuation u;			// actuation
	Actuation u_prev1;		// actuation 1 sample ago
	Error err;				// position error
	Error err_prev1;		// position error 1 sample ago
	Error err_prev2;		// position error 2 samples ago
	ErrorSum err_sum;		// cumulative position error
} ControllerState;

#endif /* CONTROL_H_ */
