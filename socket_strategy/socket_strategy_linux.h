#ifndef L2AUTH_SOCKET_STRATEGY_LINUX_H
#define L2AUTH_SOCKET_STRATEGY_LINUX_H

#include <sys/types.h>
#include <core/l2_socket.h>

int socket_strategy_linux_connect(struct L2Socket* l2_socket);
int socket_strategy_linux_close(struct L2Socket* l2_socket);
int socket_strategy_linux_bind
(
        struct L2Socket* l2_socket,
        unsigned short port
);
int socket_strategy_linux_listen(struct L2Socket* l2_socket);
int socket_strategy_linux_accept
(
        struct L2Socket* server,
        struct L2Socket* client
);
ssize_t socket_strategy_linux_receive
(
        struct L2Socket* l2_socket,
        unsigned char* buffer,
        size_t buffer_size
);
ssize_t socket_strategy_linux_send
(
        struct L2Socket* l2_socket,
        unsigned char* buffer,
        size_t buffer_size
);
void socket_strategy_linux(struct L2SocketStrategy* socket_strategy);

#endif
