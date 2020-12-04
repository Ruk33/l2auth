#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <data_structures/list.h>
#include "game_server_manager.h"

void storage_game_server_manager_init(struct StorageGameServerManager *manager)
{
        assert(manager);
        manager->servers = list_create(malloc, free);
}

void storage_game_server_manager_free(struct StorageGameServerManager *manager)
{
        assert(manager);
        assert(manager->servers);
        list_free(manager->servers);
}

void storage_game_server_manager_add(struct StorageGameServerManager *manager, struct StorageGameServer *server)
{
        assert(manager);
        assert(server);
        assert(manager->servers);

        struct StorageGameServer *server_copy = malloc(sizeof(*server_copy));
        memcpy(server_copy, server, sizeof(*server_copy));
        list_add_last(&manager->servers, server_copy);
}

void storage_game_server_manager_remove(struct StorageGameServerManager *manager, struct StorageGameServer *server)
{
        assert(manager);
        assert(server);
        assert(0 && "TODO");
}

struct List *storage_game_server_manager_servers(struct StorageGameServerManager *manager)
{
        assert(manager);
        assert(manager->servers);
        return manager->servers;
}
