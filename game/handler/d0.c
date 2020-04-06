#ifndef L2AUTH_GAME_HANDLER_D0_C
#define L2AUTH_GAME_HANDLER_D0_C

#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/handler/d0.h>

l2_packet* game_handler_d0(l2_raw_packet* request)
{
        l2_packet* packet = NULL;
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

        return packet;
}

#endif
