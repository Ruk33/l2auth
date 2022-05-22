#ifndef GAME_H
#define GAME_H

#include "../../../include/util.h"
#include "state.h"
#include "client.h"

struct client *server_on_new_connection(struct state *state);

void server_on_disconnect(struct state *state, struct client *src);

void server_on_request(struct state *state, struct client *client);

void server_on_tick(struct state *state, seconds delta);

#endif
