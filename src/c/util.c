#include "util.h"


/* Saturate a value not to exceed upper and lower bounds. */
int16_t saturate(int16_t s, int16_t lb, int16_t ub)
{
	if (s > ub)
		return ub;
	else if (s < lb)
		return lb;
	else
		return s;
}
