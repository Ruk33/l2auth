#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <stdlib.h>
#include "host.h"
#include "dto/player.h"
#include "dto/vec3.h"
#include "client_manager.h"

struct WorldManager;

void world_manager_tick(struct WorldManager *manager);

struct WorldManager *world_manager_new(host_malloc_cb m, host_mfree_cb f);

void world_manager_client_connects(struct WorldManager *manager, struct Client *client);

struct Client *world_manager_find_client_by_id(struct WorldManager *manager, int client_id);

void world_manager_spawn_char_in_world(struct WorldManager *manager, struct Client *client);

void world_manager_disconnect_client(struct WorldManager *manager, struct Client *client);

/**
 * Send current active player info from client to all
 * users connected
 */
void world_manager_display_active_char(struct WorldManager *manager, struct Client *client);

/**
 * Move current active player from client
 * and notify all connected users
 */
void world_manager_move_active_char(struct WorldManager *manager, struct Client *client, struct Vec3 *prev_location, struct Vec3 *new_location);

/**
 * Make the current active player from client talk and
 * notify the rest of the connected users
 */
void world_manager_active_char_talk(struct WorldManager *manager, struct Client *client, char *buf, size_t buf_size);

#endif
