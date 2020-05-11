#ifndef L2AUTH_GAME_ACTION_PROTOCOL_VERSION_H
#define L2AUTH_GAME_ACTION_PROTOCOL_VERSION_H

#include <core/l2_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>

void game_action_protocol_version_handler
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request
);

#endif
