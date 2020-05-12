#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <game/server.h>
#include <game/client.h>
#include "handler.h"

void game_action_d0_handler
(
        struct GameServer* server,
        struct GameClient* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);
        assert(encrypt_key);

        unsigned short id = 0;

        memcpy(&id, request + 3, sizeof(id));
        log_info("D0 ID: %d", id);

        switch (id) {
                case 8:
                        log_info("Requesting Manor List, ignoring");
                        break;
                default:
                        log_fatal("Unknown id to handle from packet D0");
                        break;
        }
}
