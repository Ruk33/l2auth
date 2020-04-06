#include <stdlib.h>
#include <time.h>
#include <log/log.h>
#include <socket_strategy/socket_strategy_linux.h>
#include <core/l2_client.h>
#include <core/l2_socket.h>
#include <core/l2_server.h>
#include <game/server.h>

int main()
{
        struct L2SocketStrategy* socket_strategy = calloc(1, sizeof(struct L2SocketStrategy));
        struct L2Socket* server = calloc(1, sizeof(struct L2Socket));

        srand((unsigned int) time(NULL));

        socket_strategy_linux(socket_strategy);
        l2_server_create(server, socket_strategy, 7777);
        log_info("Listening for connections for gameserver");

        while (1) {
                game_server_accept_and_handle_connection(server);
                break;
        }

        l2_client_close(server);
        log_info("Closing gameserver");

        return 0;
}
