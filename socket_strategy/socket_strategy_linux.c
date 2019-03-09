#ifndef L2AUTH_SOCKET_STRATEGY_LINUX_C
#define L2AUTH_SOCKET_STRATEGY_LINUX_C

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <core/l2_socket.c>

int socket_strategy_linux_connect(struct l2_socket* l2_socket)
{
        return (l2_socket->socket = socket(AF_INET, SOCK_STREAM, 0));
}

int socket_strategy_linux_bind(
        struct l2_socket* l2_socket,
        unsigned short port
)
{
        struct sockaddr_in addr;

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        return bind(
                l2_socket->socket,
                (struct sockaddr *)&addr,
                sizeof(addr)
        );
}

int socket_strategy_linux_listen(struct l2_socket* l2_socket)
{
        return listen(l2_socket->socket, 3);
}

int socket_strategy_linux_accept(
        struct l2_socket* server,
        struct l2_socket* client
)
{
        struct sockaddr_in addr;
        socklen_t addr_size = (socklen_t) sizeof(addr);
        client->socket = accept(
                server->socket,
                (struct sockaddr*) &addr,
                &addr_size
        );

        return client->socket;
}

ssize_t socket_strategy_linux_receive(
        struct l2_socket* l2_socket,
        unsigned char* buffer,
        size_t buffer_size
)
{
        return recv(l2_socket->socket, buffer, buffer_size, 0);
}

ssize_t socket_strategy_linux_send(
        struct l2_socket* l2_socket,
        unsigned char* buffer,
        size_t buffer_size
)
{
        return send(l2_socket->socket, buffer, buffer_size, 0);
}

void socket_strategy_linux(struct l2_socket_strategy* socket_strategy)
{
        socket_strategy->connect = socket_strategy_linux_connect;
        socket_strategy->bind = socket_strategy_linux_bind;
        socket_strategy->listen = socket_strategy_linux_listen;
        socket_strategy->accept = socket_strategy_linux_accept;
        socket_strategy->receive = socket_strategy_linux_receive;
        socket_strategy->send = socket_strategy_linux_send;
}

#endif //L2AUTH_SOCKET_STRATEGY_LINUX_C
