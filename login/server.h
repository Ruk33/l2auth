#ifndef L2AUTH_LOGIN_SERVER_H
#define L2AUTH_LOGIN_SERVER_H

#include <stdlib.h>

struct LoginServer;

void login_server_start_or_die(unsigned short port, size_t max_players);

#endif
