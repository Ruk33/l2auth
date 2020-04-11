#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <core/l2_socket.h>
#include <socket_strategy/socket_strategy_linux.h>

int socket_strategy_linux_connect(struct L2Socket* l2_socket)
{
        return (l2_socket->socket = socket(AF_INET, SOCK_STREAM, 0));
}

int socket_strategy_linux_close(struct L2Socket* l2_socket)
{
        shutdown(l2_socket->socket, SHUT_RDWR);
        return close(l2_socket->socket);
}

int socket_strategy_linux_bind(
        struct L2Socket* l2_socket,
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

int socket_strategy_linux_listen(struct L2Socket* l2_socket)
{
        return listen(l2_socket->socket, 3);
}

int socket_strategy_linux_accept
(
        struct L2Socket* server,
        struct L2Socket* client
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

ssize_t socket_strategy_linux_receive
(
        struct L2Socket* l2_socket,
        unsigned char* buffer,
        size_t buffer_size
)
{
        return recv(l2_socket->socket, buffer, buffer_size, 0);
}

ssize_t socket_strategy_linux_send
(
        struct L2Socket* l2_socket,
        unsigned char* buffer,
        size_t buffer_size
)
{
        return send(l2_socket->socket, buffer, buffer_size, 0);
}

void socket_strategy_linux(struct L2SocketStrategy* socket_strategy)
{
        socket_strategy->connect = socket_strategy_linux_connect;
        socket_strategy->close = socket_strategy_linux_close;
        socket_strategy->bind = socket_strategy_linux_bind;
        socket_strategy->listen = socket_strategy_linux_listen;
        socket_strategy->accept = socket_strategy_linux_accept;
        socket_strategy->receive = socket_strategy_linux_receive;
        socket_strategy->send = socket_strategy_linux_send;
}
