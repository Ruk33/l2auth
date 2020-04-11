#ifndef L2AUTH_GAME_HANDLER_REQUEST_QUESTS_H
#define L2AUTH_GAME_HANDLER_REQUEST_QUESTS_H

#include <core/l2_raw_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>

void game_handler_request_quests
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
);

#endif
