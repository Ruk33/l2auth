#include <stdlib.h>
#include <time.h>
#include <log/log.h>
#include <game/server.h>

int main()
{
        unsigned short port = 7777;
        size_t max_players = 1;

        srand((unsigned int) time(NULL));
        game_server_start_or_die(port, max_players);

        return 0;
}
