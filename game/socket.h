#ifndef SOCKET_H
#define SOCKET_H

#include <stdlib.h>
#include <sys/types.h>

/**
 * Callback to be executed
 * when a new client hits our
 * server. Use socket_accept(fd) to
 * accept and return the new client.
 * attached_data is the information
 * attached using data from socket_handle_requests.
 */
typedef int (*socket_on_conn_cb)(int fd, void *attached_data);

/**
 * Callback to be executed when
 * a new request comes from a client.
 * attached_data is the information
 * attached using data from socket_handle_requests.
 */
typedef void (*socket_on_request_cb)(int fd, void *attached_data, unsigned char *request, ssize_t request_size);

/**
 * Callback to be executed
 * when a client gets disconnected.
 * attached_data is the information
 * attached using data from socket_handle_requests.
 */
typedef void (*socket_on_disconnect_cb)(int fd, void *attached_data);

struct SocketConnection {
        int fd;
        void *data;
        socket_on_conn_cb on_connect;
        socket_on_request_cb on_request;
        socket_on_disconnect_cb on_disconnect;
};

typedef struct SocketConnection socket_conn_t;

/**
 * Create a new socket listening on port
 * accepting a maximum of max_connections
 * On success, the socket file descriptor
 * is returned. On error, -1 will be returned
 * and errno will be set
 */
int socket_create(unsigned short port, size_t max_connections);

/**
 * Accept a new connection and return its
 * file descriptor. On error, -1 will be
 * returned.
 */
int socket_accept(int fd);

/**
 * Send packet to socket.
 * On success, the number of bytes sent will be returned.
 * On error, -1 will be returned.
 */
ssize_t socket_send(int fd, unsigned char *response, size_t response_size);

/**
 * Close a socket.
 */
void socket_close(int fd);

/**
 * Start handling clients and requests for fd.
 * conn callback will be called with data when a new connection hits fd
 * req callback will be called with data when a new request hits fd
 * dis callback will be called with data when a client disconnects
 * On success, the thread will be blocked
 * to handle all request. On error, -1 will be returned.
 */
int socket_handle_requests(socket_conn_t *conn);

#endif
