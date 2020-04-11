#ifndef L2AUTH_LOGIN_SERVER_C
#define L2AUTH_LOGIN_SERVER_C

#include <log/log.h>
#include <core/l2_server.h>
#include <login/handler/client.h>
#include <login/server.h>

void login_server_start_or_die(unsigned short port, size_t max_players)
{
        struct L2Server* server = l2_server_create(max_players);

        log_info("Starting loginserver");

        if (server == NULL) {
                log_fatal("Not able to allocate memory for loginserver");
                exit(1);
        }

        l2_server_start(server, port, login_handler_client);
}

#endif
