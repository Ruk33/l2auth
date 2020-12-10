#ifndef LIB_CLIENT_REQUEST_HANDLE_H
#define LIB_CLIENT_REQUEST_HANDLE_H

#include "../headers.h"
#include "../storage/server.h"

/**
 * Entry point for all Lineage 2
 * requests made from the client.
 * The requests will be delegated
 * and handled by the files
 * located in client_request.
 */
void client_request_handle(
        int client,
        byte *request,
        size_t request_size,
        struct Host *host,
        storage_server_t *server_storage
);

#endif
