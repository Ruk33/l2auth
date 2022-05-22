#ifndef PLAYER_REQUEST_H
#define PLAYER_REQUEST_H

#include "types.h"

void player_on_request(struct state *state, struct client *client);

void player_on_tick(struct state *state, struct client *client, seconds delta);

#endif
