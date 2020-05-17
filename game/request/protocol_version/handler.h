#ifndef L2AUTH_GAME_REQUEST_PROTOCOL_VERSION_H
#define L2AUTH_GAME_REQUEST_PROTOCOL_VERSION_H

#include <core/l2_packet.h>
#include <game/server.h>
#include <game/client.h>

void game_request_protocol_version_handler
(
        struct GameServer* server,
        struct GameClient* client,
        l2_raw_packet* request
);

#endif
