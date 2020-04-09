#ifndef L2AUTH_LOGIN_SERVER_H
#define L2AUTH_LOGIN_SERVER_H

#include <stdlib.h>
#include <core/l2_client.h>

struct LoginServer;

struct LoginServer* login_server_create
(
        size_t max_players,
        size_t mem_in_bytes_per_player
);
void login_server_listen(struct LoginServer* server, unsigned short port);
void login_server_accept_client(struct LoginServer* server);

#endif
