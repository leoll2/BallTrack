/* 4-pin touchscreen */

#ifndef TOUCH_H_
#define TOUCH_H_

#include <stdbool.h>
#include "types/geometry.h"

void Touch_Config(void);
Coordinate Touch_Read(void);
bool Touch_ValidateRead(Coordinate pos, Coordinate pos_prev);

#endif // TOUCH_H_
