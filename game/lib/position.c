#include <assert.h>
#include <math.h>
#include "position.h"

unsigned int position_distance(position_t *a, position_t *b)
{
        assert(a);
        assert(b);
        int x_d = b->x - a->x;
        int y_d = b->y - a->y;
        int z_d = b->z - a->z;
        return (unsigned int) sqrt((x_d * x_d) + (y_d * y_d) + (z_d * z_d));
}
