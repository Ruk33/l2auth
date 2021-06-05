#ifndef INCLUDE_OS_SOCKET_H
#define INCLUDE_OS_SOCKET_H

#include "util.h"

typedef void os_socket_t;

typedef enum {
        CONN,
        REQ,
        CLOSED,
} socket_ev_t;

typedef void (*socket_ev_cb)(os_socket_t *s, socket_ev_t e, byte_t *b, size_t n);

// Creates a socket and binds it to a port.
// On error, 0 is returned.
os_socket_t *os_socket_create(u16_t port);

// Accept a socket connection.
// On error, 0 is returned.
// os_socket_t *socket_accept(os_socket_t *socket);

// Send n bytes from src through a specified socket.
// The amount of bytes sent is returned. On error, -1.
ssize_t os_socket_send(os_socket_t *socket, byte_t *src, size_t n);

// On success, 1 is returned. On error, 0.
int os_socket_close(os_socket_t *socket);

// Block the current thread and start handling
// request made to socket. When a new request comes,
// cb will be executed.
// On error, 0 will be returned immediately.
int os_socket_handle_requests(os_socket_t *socket, socket_ev_cb cb);

#endif
