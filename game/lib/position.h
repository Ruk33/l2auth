#ifndef LIB_POSITION_H
#define LIB_POSITION_H

struct Position {
        int x;
        int y;
        int z;
};

typedef struct Position position_t;

/**
 * Calculate distance between two points.
 */
unsigned int position_distance(position_t *a, position_t *b);

#endif
