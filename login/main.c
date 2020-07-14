#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <log/log.h>
#include <login/server.h>

int main()
{
        unsigned short port = 2106;
        size_t max_players = 2;

        srand((unsigned int) time(NULL));
        login_server_start_or_die(port, max_players);

        return 0;
}
