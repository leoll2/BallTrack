/*
 * controller.h
 *
 *  Created on: 11/feb/2019
 *      Author: leona
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <stdbool.h>
#include "types/control.h"

Position Controller_GetPosPrev1(void);
Actuation Controller_GetOutput(Position pos, Position ref_pos, bool reset);

#endif /* CONTROLLER_H_ */
