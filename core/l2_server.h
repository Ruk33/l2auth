#ifndef L2AUTH_L2_SERVER_H
#define L2AUTH_L2_SERVER_H

#include <core/l2_socket.h>

void l2_server_create
(
        struct L2Socket *server,
        struct L2SocketStrategy *socket_type,
        unsigned short port
);
void l2_server_accept_and_handle_connection(struct L2Socket *server);
void l2_server_wait_and_accept_connections(struct L2Socket *server);

#endif
