#ifndef L2AUTH_L2_SERVER_H
#define L2AUTH_L2_SERVER_H

#include <stdlib.h>
#include <core/l2_client.h>
#include <core/connection_thread.h>

struct L2Server;

typedef void (* l2_server_accept_conn_handler)(struct ConnectionThread* conn);

struct L2Server* l2_server_create(size_t max_players);
void l2_server_start(
        struct L2Server* server,
        unsigned short port,
        l2_server_accept_conn_handler handler
);
void l2_server_free(struct L2Server* server);

#endif
