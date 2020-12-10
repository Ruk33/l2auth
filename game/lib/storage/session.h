#ifndef LIB_STORAGE_SESSION_H
#define LIB_STORAGE_SESSION_H

#include "../headers.h"
#include "../session.h"

struct StorageSession {
        struct HashMap *session_storage;
        host_alloc alloc_memory;
        host_dealloc dealloc_memory;
};

typedef struct StorageSession storage_session_t;

/**
 * This function must be called
 * before using any of the others.
 */
void storage_session_init(
        storage_session_t *manager,
        host_alloc alloc_memory,
        host_dealloc dealloc_memory
);

/**
 * Register a new session.
 */
void storage_session_add(
        storage_session_t *manager,
        int session_id,
        session_t *session
);

/**
 * Remove session.
 */
void storage_session_remove(storage_session_t *manager, int session_id);

/**
 * Get session by id.
 * NULL will be returned if no
 * session can be found.
 */
session_t *storage_session_get(storage_session_t *manager, int session_id);

#endif
