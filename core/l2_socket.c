#ifndef L2AUTH_L2_SOCKET_C
#define L2AUTH_L2_SOCKET_C

#include <assert.h>
#include <sys/types.h>
#include <core/l2_socket.h>

int l2_socket_connect
(
        struct L2Socket* l2_socket,
        struct L2SocketStrategy* socket_type
)
{
        assert(l2_socket);
        assert(socket_type);
        l2_socket->strategy = socket_type;
        return l2_socket->strategy->connect(l2_socket);
}

int l2_socket_close(struct L2Socket* l2_socket)
{
        assert(l2_socket);
        return l2_socket->strategy->close(l2_socket);
}

int l2_socket_bind(struct L2Socket* l2_socket, unsigned short port)
{
        assert(l2_socket);
        return l2_socket->strategy->bind(l2_socket, port);
}

int l2_socket_listen(struct L2Socket* l2_socket)
{
        assert(l2_socket);
        return l2_socket->strategy->listen(l2_socket);
}

int l2_socket_accept(struct L2Socket* server, struct L2Socket* client)
{
        assert(server);
        assert(client);
        assert(server->strategy);
        client->strategy = server->strategy;
        return server->strategy->accept(server, client);
}

ssize_t l2_socket_receive
(
        struct L2Socket* socket,
        unsigned char* buffer, size_t buffer_size
)
{
        assert(socket);
        assert(buffer);
        return socket->strategy->receive(socket, buffer, buffer_size);
}

ssize_t l2_socket_send
(
        struct L2Socket* socket,
        unsigned char* buffer,
        size_t buffer_size
)
{
        assert(socket);
        assert(buffer);
        return socket->strategy->send(socket, buffer, buffer_size);
}

#endif
