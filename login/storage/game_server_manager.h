#ifndef STORAGE_GAME_SERVER_MANAGER_H
#define STORAGE_GAME_SERVER_MANAGER_H

#include <data_structures/list.h>
#include "game_server.h"

/**
 * This manager will store all the game
 * servers available that a player
 * can connect to. This servers refer to
 * Bartz, Siegehardt, etc.
 */
struct StorageGameServerManager {
        struct List *servers;
};

/**
 * This function must be called before
 * using any of the rest of the
 * functions.
 */
void storage_game_server_manager_init(struct StorageGameServerManager *manager);

/**
 * Release ONLY internal memory
 * used by the manager.
 */
void storage_game_server_manager_free(struct StorageGameServerManager *manager);

/**
 * Add game server to the manager.
 */
void storage_game_server_manager_add(struct StorageGameServerManager *manager, struct StorageGameServer *server);

/**
 * Remove server from manager.
 */
void storage_game_server_manager_remove(struct StorageGameServerManager *manager, struct StorageGameServer *server);

/**
 * Get all game servers registered in
 * the manager.
 */
struct List *storage_game_server_manager_servers(struct StorageGameServerManager *manager);

#endif
