#ifndef L2AUTH_OS_SOCKET_H
#define L2AUTH_OS_SOCKET_H

#include <stdlib.h>

typedef void os_socket_handler;

size_t os_socket_handler_size();

void os_socket_connect(os_socket_handler* socket_handler);

void os_socket_close(os_socket_handler* socket_handler);

void os_socket_bind
(
        os_socket_handler* socket_handler,
        unsigned short port
);

/**
 * Start listening for how_many connections
 */
void os_socket_listen(os_socket_handler* socket_handler, size_t how_many);

/**
 * Accept and create a socket for client
 */
void os_socket_accept
(
        os_socket_handler* server,
        os_socket_handler* client
);

/**
 * Receive buffer_size amount of bytes into buffer from socket
 */
size_t os_socket_receive
(
        os_socket_handler* socket_handler,
        unsigned char* buffer,
        size_t buffer_size
);

/**
 * Send buffer_size amount of bytes through socket
 */
void os_socket_send
(
        os_socket_handler* socket_handler,
        unsigned char* buffer,
        size_t buffer_size
);

#endif
