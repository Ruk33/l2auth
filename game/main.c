#include <stdlib.h>
#include <time.h>
#include <log/log.h>
#include <game/server.h>

int main()
{
        size_t max_players = 1;
        struct GameServer* server = game_server_create(max_players);
        unsigned short port = 7777;

        if (server == NULL) {
                log_fatal("Not able to allocate memory for gameserver");
                exit(1);
        }

        srand((unsigned int) time(NULL));

        log_info("Listening for connections for gameserver");
        game_server_start(server, port);
        log_info("Closing gameserver");

        return 0;
}
