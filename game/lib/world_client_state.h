#ifndef WORLD_CLIENT_STATE_H
#define WORLD_CLIENT_STATE_H

#include <stdlib.h>
#include <data_structure/list.h>
#include "dto/vec3.h"
#include "host.h"
#include "client.h"

struct WorldClientState;

struct WorldClientState *world_client_state_new(host_malloc_cb m, host_mfree_cb f);

/**
 * Spawn client in the world, notifying the rest 
 * of the connected users
 */
void world_client_state_spawn_client(struct WorldClientState *state, struct Client *client);

/**
 * Remove client from the world. It will also notify
 * the rest of the world so all clients are in sync
 */
void world_client_state_unspawn_client(struct WorldClientState *state, struct Client *client);

/**
 * Send current active player info from client to all
 * users connected
 */
void world_client_state_info(struct WorldClientState *state, struct Client *client);

/**
 * Send validate location of current active player 
 * from client to all users connected
 */
void world_client_state_validate_location(struct WorldClientState *state, struct Client *client, struct Vec3 *location, int heading);

/**
 * Move current active player from client
 * and notify all connected users
 */
void world_client_state_move(struct WorldClientState *state, struct Client *client, struct Vec3 *prev_location, struct Vec3 *new_location);

/**
 * Make a client talk!
 * A say_packet will be sent to all clients
 */
void world_client_state_say(struct WorldClientState *state, struct Client *client, char *buf, size_t buf_size);

/**
 * Get a copy of the clients. It must be deallocated.
 */
struct ListEntry *world_client_state_get_all(struct WorldClientState *state);

/**
 * Find a client by id. If the client doesn't exist,
 * NULL is returned
 */
struct Client *world_client_state_find(struct WorldClientState *state, int client_id);

#endif
