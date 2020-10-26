#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include <stdlib.h>
#include "host.h"
#include "dto/player.h"
#include "dto/vec3.h"
#include "world_client_state.h"

struct WorldState;

struct WorldState *world_state_new(host_malloc_cb m, host_mfree_cb f);

struct Client *world_state_find_client(struct WorldState *world, int client_id);

void world_state_spawn_client(struct WorldState *world, struct Client *client);

void world_state_unspawn_client(struct WorldState *world, struct Client *client);

/**
 * Send current active player info from client to all
 * users connected
 */
void world_state_client_info(struct WorldState *world, struct Client *client);

/**
 * Send validate location of current active player 
 * from client to all users connected
 */
void world_state_client_validate_location(struct WorldState *world, struct Client *client, struct Vec3 *location, int heading);

/**
 * Move current active player from client
 * and notify all connected users
 */
void world_state_client_move(struct WorldState *world, struct Client *client, struct Vec3 *prev_location, struct Vec3 *new_location);

/**
 * Make the current active player from client talk and
 * notify the rest of the connected users
 */
void world_state_client_say(struct WorldState *world, struct Client *client, char *buf, size_t buf_size);

#endif
