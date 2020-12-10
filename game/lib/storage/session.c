#include "../headers.h"
#include "../session.h"
#include "session.h"

void storage_session_init(
        storage_session_t *manager,
        host_alloc alloc_memory,
        host_dealloc dealloc_memory
)
{
        assert(manager);
        assert(alloc_memory);
        assert(dealloc_memory);

        manager->alloc_memory = alloc_memory;
        manager->dealloc_memory = dealloc_memory;
        manager->session_storage = hash_map_create(alloc_memory, dealloc_memory, 50);
}

void storage_session_add(
        storage_session_t *manager,
        int session_id,
        session_t *session
)
{
        assert(manager);
        assert(manager->alloc_memory);
        assert(manager->session_storage);
        assert(session_id > 0);
        assert(session);

        session_t *session_copy = manager->alloc_memory(sizeof(*session_copy));
        char key[8] = {0};

        snprintf(key, sizeof(key), "%d", session_id);
        memcpy(session_copy, session, sizeof(*session_copy));
        hash_map_set(manager->session_storage, key, strlen(key) + 1, session_copy);
}

void storage_session_remove(storage_session_t *manager, int session_id)
{
        assert(manager);
        assert(session_id > 0);

        char key[8] = {0};
        snprintf(key, sizeof(key), "%d", session_id);
        hash_map_set(manager->session_storage, key, strlen(key) + 1, NULL);
}

session_t *storage_session_get(storage_session_t *manager, int session_id)
{
        assert(manager);
        assert(manager->session_storage);
        assert(session_id > 0);

        char key[8] = {0};
        snprintf(key, sizeof(key), "%d", session_id);
        return hash_map_get(manager->session_storage, key, strlen(key) + 1);
}
