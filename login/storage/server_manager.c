#include <assert.h>
#include "session_manager.h"
#include "game_server_manager.h"
#include "server_manager.h"

void storage_server_manager_init(struct StorageServerManager *manager)
{
        assert(manager);
        storage_game_server_manager_init(&manager->game_server_manager);
        storage_session_manager_init(&manager->session_manager);
}

void storage_server_manager_free(struct StorageServerManager *manager)
{
        assert(manager);
        assert(&manager->game_server_manager);
        assert(&manager->session_manager);
        storage_game_server_manager_free(&manager->game_server_manager);
        storage_session_manager_free(&manager->session_manager);
}
