#ifndef L2AUTH_LOGIN_GAME_SERVER_H
#define L2AUTH_LOGIN_GAME_SERVER_H

#include <core/l2_socket.h>

void game_server_accept_and_handle_connection(struct L2Socket *server);

#endif
