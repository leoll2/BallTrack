#include "field.h"

/* Target location */
const Box target_box = {18, 1};


/* If indices are out-of-bounds, assign them boundary values. */
static void fix_box_indices(Box *b)
{
	if (b->i < 0)
		b->i = 0;
	else if (b->i >= N_BOX_X)
		b->i = N_BOX_X - 1;

	if (b->j < 0)
		b->j = 0;
	else if (b->j >= N_BOX_Y)
		b->j = N_BOX_Y - 1;
}



/* Compare two Box for equality */
bool Field_IsEqualBox(Box b1, Box b2)
{
	return ((b1.i == b2.i) && (b1.j == b2.j));
}


/* Given box (x,y), compute the next one (xx,yy) to reach. */
Box Field_GetBoxFromDelta(Box cur, int16_t delta_i, int16_t delta_j)
{
	Box next;
	next.i = cur.i + delta_i;
	next.j = cur.j + delta_j;
	fix_box_indices(&next);
	return next;
}


Coordinate Field_GetMiddleCoordOfBox(Box box)
{
	Coordinate p;
	p.x = MIN_X_COORD + box.i * UNIT_X_COORD + (UNIT_X_COORD/2);
	p.y = MIN_Y_COORD + box.j * UNIT_Y_COORD + (UNIT_Y_COORD/2);
	return p;
}


Box Field_GetBoxFromCoord(Coordinate c)
{
	Box b;
	b.i = (c.x - MIN_X_COORD) / UNIT_X_COORD;
	b.j = (c.y - MIN_Y_COORD) / UNIT_Y_COORD;
	return b;
}

