#ifndef LIB_STATE_MACHINE_HANDLE_H
#define LIB_STATE_MACHINE_HANDLE_H

#include <headers.h>
#include <db/conn.h>
#include <host.h>

/**
 * Entry point for client request.
 */
void state_machine_handle(host_t *h, int socket, byte_t *request, ssize_t size);

#endif
