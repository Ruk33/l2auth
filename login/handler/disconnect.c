#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../storage/server_manager.h"
#include "../storage/session_manager.h"
#include "disconnect.h"

void handler_disconnect(struct StorageServerManager *server_manager, int fd)
{
        assert(server_manager);
        assert(fd > 0);
        printf("Client removed from session manager.\n");
        storage_session_manager_remove(&server_manager->session_manager, fd);
}
