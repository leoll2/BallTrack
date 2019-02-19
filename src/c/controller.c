/*
 * controller.c
 *
 *  Created on: 11/feb/2019
 *      Author: leona
 */

#include <math.h>
#include <stdlib.h>
#include "stm32f4xx.h"

#include "controller.h"
#include "util.h"


ControllerState st = {
	.pos_prev1 = {0, 0},
	.u = {0, 0},
	.u_prev1 = {0, 0},
	.err = {0, 0},
	.err_prev1 = {0, 0},
	.err_prev2 = {0, 0},
	.err_sum = {0, 0}
};


/* Update the digital control quantities (errors, previous values, and so on).
 * If reset is specified, some values are reinitialized or adjusted. */
void compute_control_variables(Position pos, Position ref_pos, bool reset)
{
	int16_t temp1, temp2;
	st.pos_prev1 = pos;

	if (reset) {
		// partial reset of cumulative error: tradeoff to avoid
		// both windup and abrupt output transitions
		st.err_sum.x /= 2;
		st.err_sum.y /= 2;
		// when changing target box, the error suddenly increases;
		// previous errors shall be incremented accordingly, because
		// their value was computed in the previous box
		temp1 = st.err_prev1.x - st.err.x;
		temp2 = st.err_prev2.x - st.err_prev1.x;
		st.err.x = ref_pos.x - pos.x;
		st.err_prev1.x = st.err.x + temp1;
		st.err_prev2.x = st.err_prev1.x + temp2;
		temp1 = st.err_prev1.y - st.err.y;
		temp2 = st.err_prev2.y - st.err_prev1.y;
		st.err.y = ref_pos.y - pos.y;
		st.err_prev1.y = st.err.y + temp1;
		st.err_prev2.y = st.err_prev1.y + temp2;

		st.u_prev1.x = st.u_prev1.y = 0;
	} else {
		st.err_prev2.x = st.err_prev1.x;
		st.err_prev2.y = st.err_prev1.y;
		st.err_prev1.x = st.err.x;
		st.err_prev1.y = st.err.y;
		st.err.x = ref_pos.x - pos.x;
		st.err.y = ref_pos.y - pos.y;
		st.u_prev1.x = saturate(st.u.x, -100, 100);
		st.u_prev1.y = saturate(st.u.y, -100, 100);
	}
}


/* Controller proportional to error, error derivative and double-derivative.
 * u[k] = Kp*e[k] + Kd*(e[k]-e[k-1]) + Ka*((e[k]-e[k-1])-(e[k-1]-e[k-2]))
 * u[k] = Kp*e[k] + Kd*(e[k]-e[k-1]) + Ka*((e[k]-2*e[k-1]+e[k-2]))
 * u[k] = (Kp+Kd+Ka)*e[k] + (-Kd-2*Ka)*e[k-1] + Ka*e[k-2]
 * There is also an additive (non-linear) integral action, which helps
 * rejecting static error. This component is reset when changing reference. */
#define Kp  	(7.0/200)		// proportional to error
#define Kd  	(100.0/200)		// proportional to error variation (speed)
#define Ka  	(10.0/200)		// proportional to speed variation (acceleration)
#define Ki 		(2.0/15.0) 		// proportional to cumulative error
Actuation pddi_control(void)
{
	static const double a = (Kp + Kd + Ka);
	static const double b = -(Kd + 2*Ka);
	static const double c = Ka;
	static const double d = Ki;

	st.err_sum.x = saturate(st.err_sum.x + st.err.x/20, -250, 250);
	st.err_sum.y = saturate(st.err_sum.y + st.err.y/12, -250, 250);

	st.u.x = floor(a*st.err.x + b*st.err_prev1.x + c*st.err_prev2.x + d*st.err_sum.x);
	st.u.y = floor(a*st.err.y + b*st.err_prev1.y + c*st.err_prev2.y + d*st.err_sum.y);

	return st.u;
}


Position Controller_GetPosPrev1(void)
{
	return st.pos_prev1;
}


Actuation Controller_GetOutput(Position pos, Position ref_pos, bool reset)
{
	// Compute new state
	compute_control_variables(pos, ref_pos, reset);

	// Generate output accordingly
	return pddi_control();
}
