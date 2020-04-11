#ifndef L2AUTH_GAME_HANDLER_PROTOCOL_VERSION_H
#define L2AUTH_GAME_HANDLER_PROTOCOL_VERSION_H

#include <core/l2_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <game/packet/crypt_init.h>

void game_handler_protocol_version
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request
);

#endif
