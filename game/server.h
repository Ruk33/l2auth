#ifndef L2AUTH_GAME_SERVER_H
#define L2AUTH_GAME_SERVER_H

#include <stdlib.h>
#include <core/l2_socket.h>

struct GameServer;

struct GameServer* game_server_create(size_t max_players);
void game_server_start(struct GameServer* server, unsigned short port);
void game_server_free(struct GameServer* server);

#endif
