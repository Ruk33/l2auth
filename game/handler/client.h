#ifndef L2AUTH_GAME_HANDLER_CLIENT_H
#define L2AUTH_GAME_HANDLER_CLIENT_H

#include <core/l2_client.h>
#include <game/server.h>

void game_handler_client
(
        struct GameServer* server,
        struct L2Client* client
);

#endif
