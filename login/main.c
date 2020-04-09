#include <stdlib.h>
#include <time.h>
#include <log/log.h>
#include <login/server.h>

int main()
{
        size_t mem_in_bytes_per_player = 1024 * 1024;
        struct LoginServer* server = login_server_create(1, mem_in_bytes_per_player);
        unsigned short port = 2106;

        srand((unsigned int) time(NULL));

        log_info("Starting loginserver");

        if (server == NULL) {
                log_fatal("Not able to allocate memory for loginserver");
                exit(1);
        }

        login_server_listen(server, port);
        login_server_accept_client(server);

        return 0;
}
