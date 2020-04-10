#ifndef L2AUTH_LOGIN_SERVER_H
#define L2AUTH_LOGIN_SERVER_H

#include <stdlib.h>
#include <core/l2_client.h>

struct LoginServer;

struct LoginServer* login_server_create(size_t max_players);
void login_server_start(struct LoginServer* server, unsigned short port);

#endif
