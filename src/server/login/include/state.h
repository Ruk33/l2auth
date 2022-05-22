#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include "client.h"

struct state {
	struct client clients[32];
	size_t count;
	size_t recycled_clients[32];
};

struct client *state_get_free_client(struct state *src);

void state_release_client(struct state *state, struct client *src);

#endif
