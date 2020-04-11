#ifndef L2AUTH_GAME_HANDLER_D0_C
#define L2AUTH_GAME_HANDLER_D0_C

#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <game/handler/d0.h>

void game_handler_d0
(
        struct L2Server* server,
        struct L2Client* client,
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

#endif
