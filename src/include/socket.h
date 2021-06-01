#ifndef INCLUDE_SOCKET_H
#define INCLUDE_SOCKET_H

#include "util.h"

typedef void socket_t;

typedef enum {
        CONN,
        REQ,
        CLOSED,
} socket_ev_t;

typedef void (*socket_ev_cb)(socket_t *s, socket_ev_t e, byte_t *b, size_t n);

// Creates a socket and binds it to a port.
// On error, 0 is returned.
socket_t *socket_create(u16_t port);

// Accept a socket connection.
// On error, 0 is returned.
// socket_t *socket_accept(socket_t *socket);

// Send n bytes from src through a specified socket.
// The amount of bytes sent is returned. On error, -1.
ssize_t socket_send(socket_t *socket, byte_t *src, size_t n);

// On success, 1 is returned. On error, 0.
int socket_close(socket_t *socket);

// Block the current thread and start handling
// request made to socket. When a new request comes,
// cb will be executed.
// On error, 0 will be returned immediately.
int socket_handle_requests(socket_t *socket, socket_ev_cb cb);

#endif
