#include <socket_strategy/socket_strategy_linux.c>
#include <core/l2_socket.c>
#include <core/l2_server.c>

int main()
{
        struct l2_socket_strategy socket_strategy;
        struct l2_socket server;

        socket_strategy_linux(&socket_strategy);

        l2_server_create(&server, &socket_strategy, 2106);
        l2_server_wait_and_accept_connections(&server);

        return 0;
}
