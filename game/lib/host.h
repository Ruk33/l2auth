#ifndef LIB_CONNECTION_H
#define LIB_CONNECTION_H

#include <stdlib.h>

/**
 * Allocate heap memory.
 */
typedef void *(*host_alloc)(size_t how_much);

/**
 * Deallocate heap memory.
 */
typedef void (*host_dealloc)(void *memory);

/**
 * Allows to send a response to client by id.
 */
typedef void (*host_send_response_cb)(
        int client,
        unsigned char *response,
        size_t response_size
);

/**
 * Allows to disconnect a client by id.
 */
typedef void (*host_disconnect_cb)(int client);

struct Host {
        host_alloc alloc_memory;
        host_dealloc dealloc_memory;
        host_send_response_cb send_response;
        host_disconnect_cb disconnect_connection;
};

#endif
