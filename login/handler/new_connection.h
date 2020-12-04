#ifndef HANDLER_NEW_CONNECTION_H
#define HANDLER_NEW_CONNECTION_H

#include "../storage/server_manager.h"

/**
 * Handle new client connecting
 * to the server.
 */
void handler_new_connection(struct StorageServerManager *server_manager, int fd);

#endif
