#ifndef HANDLER_DISCONNECT_H
#define HANDLER_DISCONNECT_H

#include <stdlib.h>
#include "../storage/server_manager.h"

/**
 * Handle client disconnecting
 * the server.
 */
void handler_disconnect(struct StorageServerManager *server_manager, int fd);

#endif
