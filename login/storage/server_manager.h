#ifndef STORAGE_SERVER_MANAGER_H
#define STORAGE_SERVER_MANAGER_H

#include "game_server_manager.h"
#include "session_manager.h"

/**
 * This struct/manager is meant
 * to hold all the information
 * regarding the server. Some
 * of this information are the
 * client sessions.
 * It could very well be implemented
 * using services such as redis or
 * even any sql database, but, for
 * the time being, in memory will be.
 */
struct StorageServerManager {
        struct StorageGameServerManager game_server_manager;
        struct StorageSessionManager session_manager;
};

/**
 * This function must be called before
 * using any of the rest.
 */
void storage_server_manager_init(struct StorageServerManager *manager);

/**
 * Release ONLY internal memory
 * used by the manager.
 */
void storage_server_manager_free(struct StorageServerManager *manager);

#endif
