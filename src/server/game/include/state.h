#ifndef STATE_H
#define STATE_H

#include "types.h"

struct client *state_get_free_client(struct state *src);

void state_add_player(struct state *state, struct client *client, struct character *src);

struct character *state_add_character(struct state *state, struct character *src);

void state_release_client(struct state *state, struct client *src);

void state_broadcast_packet(struct state *state, struct packet *src);

#endif
