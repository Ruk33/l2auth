#ifndef CLIENT_REQUEST_HANDLE_H
#define CLIENT_REQUEST_HANDLE_H

#include <stdlib.h>
#include "../storage/server_manager.h"

/**
 * Main entry point. All request
 * will be handled by this function 
 * and later delegated to the
 * handlers from client_request.
 * Delegation depends on the packet
 * type sent by the client.
 * For a list of packet types see
 * client_packet/type.h.
 */
void client_request_handle(struct StorageServerManager *server_manager, int fd, unsigned char *request, size_t request_size);

#endif
