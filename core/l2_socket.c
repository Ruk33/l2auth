#ifndef L2AUTH_L2_SOCKET_C
#define L2AUTH_L2_SOCKET_C

#define L2_SOCKET_WIN_WSA_STARTUP_ERROR 1
#define L2_SOCKET_WIN_CREATE_SOCKET_ERROR 2
#define L2_SOCKET_WIN_BIND_ERROR 3
#define L2_SOCKET_WIN_LISTEN_ERROR 4
#define L2_SOCKET_WIN_ACCEPT_ERROR 5

#include <sys/types.h>

struct l2_socket
{
        int socket;
        struct l2_socket_strategy* strategy;
};

struct l2_socket_strategy
{
        int (* connect)(struct l2_socket* l2_socket);
        int (* close)(struct l2_socket* l2_socket);
        int (* bind)(struct l2_socket* l2_socket, unsigned short port);
        int (* listen)(struct l2_socket* l2_socket);
        int (* accept)(struct l2_socket* server, struct l2_socket* client);
        ssize_t (* receive)(struct l2_socket* l2_socket, unsigned char* buffer, size_t buffer_size);
        ssize_t (* send)(struct l2_socket* l2_socket, unsigned char* buffer, size_t buffer_size);
};

int l2_socket_connect(struct l2_socket* l2_socket, struct l2_socket_strategy* socket_type)
{
        l2_socket->strategy = socket_type;
        return l2_socket->strategy->connect(l2_socket);
}

int l2_socket_close(struct l2_socket* l2_socket)
{
        return l2_socket->strategy->close(l2_socket);
}

int l2_socket_bind(struct l2_socket* l2_socket, unsigned short port)
{
        return l2_socket->strategy->bind(l2_socket, port);
}

int l2_socket_listen(struct l2_socket* l2_socket)
{
        return l2_socket->strategy->listen(l2_socket);
}

int l2_socket_accept(struct l2_socket* server, struct l2_socket* client)
{
        client->strategy = server->strategy;
        return server->strategy->accept(server, client);
}

ssize_t l2_socket_receive(struct l2_socket* socket, unsigned char* buffer, size_t buffer_size)
{
        return socket->strategy->receive(socket, buffer, buffer_size);
}

ssize_t l2_socket_send(struct l2_socket* socket, unsigned char* buffer, size_t buffer_size)
{
        return socket->strategy->send(socket, buffer, buffer_size);
}

#endif //L2AUTH_L2_SOCKET_C
