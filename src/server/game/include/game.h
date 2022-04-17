#ifndef GAME_H
#define GAME_H

#include "../../../include/util.h"
#include "state.h"
#include "client.h"

struct client *game_on_new_connection(struct state *state);

void game_on_disconnect(struct state *state, struct client *src);

void game_on_request(struct state *state, struct client *client);

void game_on_tick(struct state *state, seconds delta);

#endif