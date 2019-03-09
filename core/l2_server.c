#ifndef L2AUTH_L2_SERVER_C
#define L2AUTH_L2_SERVER_C

#include <core/l2_socket.c>

void l2_server_create(
        struct l2_socket *server,
        struct l2_socket_strategy *socket_type,
        unsigned short port
)
{
        l2_socket_connect(server, socket_type);
        l2_socket_bind(server, port);
        l2_socket_listen(server);
}

#endif //L2AUTH_L2_SERVER_C
