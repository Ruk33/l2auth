#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <data_structures/hash_map.h>
#include "session.h"
#include "../rsa.h"
#include "../blowfish.h"
#include "session_manager.h"

void storage_session_manager_init(struct StorageSessionManager *manager)
{
        assert(manager);
        manager->sessions = hash_map_create(malloc, free, 25);
}

void storage_session_manager_free(struct StorageSessionManager *manager)
{
        assert(manager);
        hash_map_free(manager->sessions);
}

static void storage_session_manager_set(struct StorageSessionManager *manager, int id, struct StorageSession *session)
{
        assert(manager);
        assert(id > 0);

        char id_key[8] = {0};
        snprintf(id_key, sizeof(id_key), "%d", id);
        hash_map_set(manager->sessions, id_key, strlen(id_key) + 1, session);
}

void storage_session_manager_add(struct StorageSessionManager *manager, int id)
{
        assert(manager);
        assert(manager->sessions);

        struct StorageSession *session = malloc(sizeof(*session));

        session->rsa_decrypted = 0;

        rsa_init(&session->rsa);
        blowfish_init(&session->blowfish);

        storage_session_manager_set(manager, id, session);
}

struct StorageSession *storage_session_manager_get(struct StorageSessionManager *manager, int id)
{
        assert(manager);
        assert(manager->sessions);

        char id_key[8] = {0};
        snprintf(id_key, sizeof(id_key), "%d", id);
        return hash_map_get(manager->sessions, id_key, strlen(id_key) + 1);
}

void storage_session_manager_remove(struct StorageSessionManager *manager, int id)
{
        struct StorageSession *session = storage_session_manager_get(manager, id);

        if (!session)
        {
                return;
        }

        rsa_free(&session->rsa);
        free(session);

        storage_session_manager_set(manager, id, NULL);
}
