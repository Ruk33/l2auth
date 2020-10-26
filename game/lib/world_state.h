#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include <stdlib.h>
#include "host.h"
#include "dto/player.h"
#include "world_client_state.h"

struct WorldState;

struct WorldState *world_state_new(host_malloc_cb m, host_mfree_cb f);

struct Client *world_state_find_client(struct WorldState *world, int client_id);

void world_state_spawn_client(struct WorldState *world, struct Client *client);

void world_state_unspawn_client(struct WorldState *world, struct Client *client);

void world_state_client_say(struct WorldState *world, struct Client *client, char *buf, size_t buf_size);

#endif
