#ifndef LIB_STORAGE_SERVER_H
#define LIB_STORAGE_SERVER_H

#include "../headers.h"
#include "session.h"
#include "character.h"

/**
 * This struct/manager is meant
 * to hold all the information
 * regarding the server. Some
 * of this information are the
 * client session_storage.
 * It could very well be implemented
 * using services such as redis or
 * even any sql database, but, for
 * the time being, in memory will be.
 */
struct StorageServer {
        storage_session_t session_storage;
        storage_character_t character_storage;
};

typedef struct StorageServer storage_server_t;

/**
 * This function must be called before
 * using any of the rest.
 */
void storage_server_init(storage_server_t *manager, host_alloc alloc_memory, host_dealloc dealloc_memory);

#endif
