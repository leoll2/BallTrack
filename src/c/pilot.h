/*
 * pilot.h
 *
 *  Created on: 11/feb/2019
 *      Author: leona
 */

#ifndef PILOT_H_
#define PILOT_H_

#include "field.h"

#define N_ACTION 	8

typedef uint8_t ActionCode;

ActionCode Pilot_GetBestAction(Box b);
Box Pilot_GetNextBox(Box cur, ActionCode a);

#endif /* PILOT_H_ */
