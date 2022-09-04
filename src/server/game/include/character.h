#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

void move_character_towards(struct character *src, struct v3 position);

void update_character(struct state *state, struct character *src, seconds delta);

#endif
