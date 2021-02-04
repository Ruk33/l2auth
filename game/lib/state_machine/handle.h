#ifndef LIB_STATE_MACHINE_HANDLE_H
#define LIB_STATE_MACHINE_HANDLE_H

#include <headers.h>
#include <storage/server.h>

/**
 * Entry point for handling the state of the
 * user's session.
 */
void state_machine_handle(int client, byte_t *request, ssize_t request_size, host_t *host, storage_server_t *server_storage);

#endif
