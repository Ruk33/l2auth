#ifndef WORLD_CLIENT_STATE_H
#define WORLD_CLIENT_STATE_H

#include <stdlib.h>
#include <data_structure/list.h>
#include "dto/vec3.h"
#include "host.h"
#include "client.h"

struct ClientManager;

struct ClientManager *client_manager_new(host_malloc_cb m, host_mfree_cb f);

/**
 * New client gets connected, make the 
 * manager get aware of it
 */
void client_manager_client_connects(struct ClientManager *manager, struct Client *client);

/**
 * Spawn client's character in the world, notifying the rest 
 * of the connected users
 */
void client_manager_spawn_char_in_world(struct ClientManager *manager, struct Client *client);

/**
 * Remove client's char from the world. It will also notify
 * the rest of the world so all clients are in sync
 */
void client_manager_disconnect_client(struct ClientManager *manager, struct Client *client);

/**
 * Make the active character of client visible 
 * to all connected users
 */
void client_manager_display_active_char(struct ClientManager *manager, struct Client *client);

/**
 * Move current active player from client
 * and notify all connected users
 */
void client_manager_move_active_char(struct ClientManager *manager, struct Client *client, struct Vec3 *prev_location, struct Vec3 *new_location);

/**
 * Make a client talk!
 * A say_packet will be sent to all clients
 */
void client_manager_active_char_talk(struct ClientManager *manager, struct Client *client, char *buf, size_t buf_size);

/**
 * Get a copy of the clients. It must be deallocated.
 */
struct ListEntry *client_manager_get_clients(struct ClientManager *manager);

/**
 * Find a client by id and get a copy of it (must be deallocated).
 * If the client doesn't exist, NULL is returned
 */
struct Client *client_manager_client_by_id(struct ClientManager *manager, int client_id);

#endif
