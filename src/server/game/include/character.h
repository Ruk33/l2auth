#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

void character_start_movement(struct character *src, struct character_position *position);

void character_tick(struct character *src, seconds delta);

#endif
