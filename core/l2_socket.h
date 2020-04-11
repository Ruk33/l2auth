#ifndef L2AUTH_L2_SOCKET_H
#define L2AUTH_L2_SOCKET_H

#include <sys/types.h>

#define L2_SOCKET_WIN_WSA_STARTUP_ERROR 1
#define L2_SOCKET_WIN_CREATE_SOCKET_ERROR 2
#define L2_SOCKET_WIN_BIND_ERROR 3
#define L2_SOCKET_WIN_LISTEN_ERROR 4
#define L2_SOCKET_WIN_ACCEPT_ERROR 5

struct L2Socket
{
        int socket;
        struct L2SocketStrategy* strategy;
};

struct L2SocketStrategy
{
        int (* connect)(struct L2Socket* l2_socket);
        int (* close)(struct L2Socket* l2_socket);
        int (* bind)(struct L2Socket* l2_socket, unsigned short port);
        int (* listen)(struct L2Socket* l2_socket);
        int (* accept)(struct L2Socket* server, struct L2Socket* client);
        ssize_t (* receive)(struct L2Socket* l2_socket, unsigned char* buffer, size_t buffer_size);
        ssize_t (* send)(struct L2Socket* l2_socket, unsigned char* buffer, size_t buffer_size);
};

int l2_socket_connect
(
        struct L2Socket* l2_socket,
        struct L2SocketStrategy* socket_type
);

int l2_socket_close(struct L2Socket* l2_socket);

int l2_socket_bind(struct L2Socket* l2_socket, unsigned short port);

int l2_socket_listen(struct L2Socket* l2_socket);

int l2_socket_accept(struct L2Socket* server, struct L2Socket* client);

ssize_t l2_socket_receive
(
        struct L2Socket* socket,
        unsigned char* buffer,
        size_t buffer_size
);

ssize_t l2_socket_send
(
        struct L2Socket* socket,
        unsigned char* buffer,
        size_t buffer_size
);

#endif
