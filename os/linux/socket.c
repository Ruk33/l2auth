#include <assert.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <os/socket.h>

size_t os_socket_handler_size()
{
        return sizeof(int);
}

void os_socket_connect(os_socket_handler *socket_handler)
{
        assert(socket_handler);
        *((int *) socket_handler) = socket(
                AF_INET,
                SOCK_STREAM,
                0
        );
}

void os_socket_close(os_socket_handler *socket_handler)
{
        assert(socket_handler);
        int _socket = *((int *) socket_handler);

        shutdown(_socket, SHUT_RDWR);
        close(_socket);
}

void os_socket_bind
(
        os_socket_handler *socket_handler,
        unsigned short port
)
{
        assert(socket_handler);

        int _socket = *((int *) socket_handler);
        struct sockaddr_in addr;

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        bind(_socket, (struct sockaddr *) &addr, sizeof(addr));
}

void os_socket_listen
(
        os_socket_handler *socket_handler,
        size_t how_many
)
{
        assert(socket_handler);
        assert(how_many > 0);
        int _socket = *((int *) socket_handler);
        listen(_socket, (int) how_many);
}

void os_socket_accept
(
        os_socket_handler *server,
        os_socket_handler *client
)
{
        assert(server);
        assert(client);

        int server_socket = *((int *) server);
        struct sockaddr_in addr;
        socklen_t addr_size = (socklen_t) sizeof(addr);

        *((int *) client) = accept(
                server_socket,
                (struct sockaddr *) &addr,
                &addr_size
        );
}

size_t os_socket_receive
(
        os_socket_handler* socket_handler,
        unsigned char* buffer,
        size_t buffer_size
)
{
        assert(socket_handler);
        assert(buffer);
        assert(buffer_size > 0);

        int _socket = *((int *) socket_handler);
        return (size_t) recv(_socket, buffer, buffer_size, 0);
}

void os_socket_send
(
        os_socket_handler* socket_handler,
        unsigned char* buffer,
        size_t buffer_size
)
{
        assert(socket_handler);
        assert(buffer);
        assert(buffer_size > 0);

        int _socket = *((int *) socket_handler);
        send(_socket, buffer, buffer_size, 0);
}
