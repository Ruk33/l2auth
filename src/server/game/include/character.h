#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"
#include "client.h"

struct character {
    int tmp;
};

void character_move(struct client *src, struct char_pos *position);

#endif
