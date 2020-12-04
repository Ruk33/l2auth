#ifndef HANDLER_NEW_REQUEST_H
#define HANDLER_NEW_REQUEST_H

#include <stdlib.h>
#include "../storage/server_manager.h"

/**
 * Handle new request made by
 * client.
 */
void handler_new_request(struct StorageServerManager *server_manager, int fd, unsigned char *request, size_t request_size);

#endif
