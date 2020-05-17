#ifndef L2AUTH_GAME_REQUEST_H
#define L2AUTH_GAME_REQUEST_H

#include <core/l2_packet.h>
#include <game/connection.h>

struct GameRequest {
        struct GameConnection* conn;
        l2_packet* packet;
};

#endif
