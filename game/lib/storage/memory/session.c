#include <headers.h>
#include <session.h>
#include <storage/session.h>

struct StorageSessionMemory {
        struct HashMap *sessions_by_id;
        struct List *sessions;
};

typedef struct StorageSessionMemory storage_handler_t;

void storage_session_init(storage_session_t *manager, host_alloc alloc_memory, host_dealloc dealloc_memory)
{
        assert(manager);
        assert(alloc_memory);
        assert(dealloc_memory);

        storage_handler_t *handler = NULL;

        handler = alloc_memory(sizeof(*handler));
        handler->sessions_by_id = hash_map_create(alloc_memory, dealloc_memory, 50);
        handler->sessions = list_create(alloc_memory, dealloc_memory);

        manager->alloc_memory = alloc_memory;
        manager->dealloc_memory = dealloc_memory;
        manager->handler = handler;
}

void storage_session_add(storage_session_t *manager, int session_id, session_t *session)
{
        assert(manager);
        assert(manager->alloc_memory);
        assert(manager->handler);
        assert(session_id > 0);
        assert(session);

        storage_handler_t *handler = NULL;
        session_t *session_copy = NULL;
        char key[8] = {0};

        handler = manager->handler;
        session_copy = manager->alloc_memory(sizeof(*session_copy));

        snprintf(key, sizeof(key), "%d", session_id);
        memcpy(session_copy, session, sizeof(*session_copy));
        hash_map_set(handler->sessions_by_id, key, strlen(key) + 1, session_copy);
        list_add_last(&handler->sessions, session_copy);
}

void storage_session_remove(storage_session_t *manager, int session_id)
{
        assert(manager);
        assert(manager->handler);
        assert(session_id > 0);

        storage_handler_t *handler = NULL;
        session_t *session = NULL;
        struct ListEntry *entry = NULL;
        char key[8] = {0};

        handler = manager->handler;
        session = storage_session_get(manager, session_id);

        if (!session) {
                return;
        }

        snprintf(key, sizeof(key), "%d", session_id);
        hash_map_set(handler->sessions_by_id, key, strlen(key) + 1, NULL);

        entry = storage_session_all(manager);
        while (list_has_next(entry) && list_get_value(entry) != session) {
                entry = list_get_next(entry);
        }
        list_remove(&handler->sessions, entry);

        manager->dealloc_memory(session);
}

session_t *storage_session_get(storage_session_t *manager, int session_id)
{
        assert(manager);
        assert(manager->handler);
        assert(session_id > 0);

        storage_handler_t *handler = NULL;
        char key[8] = {0};

        handler = manager->handler;
        snprintf(key, sizeof(key), "%d", session_id);

        return hash_map_get(handler->sessions_by_id, key, strlen(key) + 1);
}

struct ListEntry *storage_session_all(storage_session_t *manager)
{
        assert(manager);
        assert(manager->handler);
        storage_handler_t *handler = NULL;
        handler = manager->handler;
        return list_get_iterator(handler->sessions);
}
