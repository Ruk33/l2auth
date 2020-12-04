#ifndef STORAGE_SESSION_MANAGER_H
#define STORAGE_SESSION_MANAGER_H

#include <data_structures/hash_map.h>
#include "session.h"

struct StorageSessionManager {
        struct HashMap *sessions;
};

/**
 * This function must be called before
 * using any of the rest of the 
 * functions.
 */
void storage_session_manager_init(struct StorageSessionManager *manager);

/**
 * Free ONLY internal memory
 * used by the manager.
 */
void storage_session_manager_free(struct StorageSessionManager *manager);

/**
 * Add a new session using
 * a session id.
 */
void storage_session_manager_add(struct StorageSessionManager *manager, int id);

/**
 * Remove session from manager.
 */
void storage_session_manager_remove(struct StorageSessionManager *manager, int id);

/**
 * Get session by id. NULL may be
 * returned in which case it means
 * the session doesn't exist at all or 
 * the client was disconnected and thus,
 * removed from the manager.
 */
struct StorageSession *storage_session_manager_get(struct StorageSessionManager *manager, int id);

#endif
