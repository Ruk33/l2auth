#ifndef L2AUTH_GAME_HANDLER_ENTER_WORLD_C
#define L2AUTH_GAME_HANDLER_ENTER_WORLD_C

#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/packet/enter_world.h>
#include <game/handler/enter_world.h>

l2_packet* game_handler_enter_world(l2_raw_packet* request)
{
        l2_packet* packet = game_packet_enter_world();
        log_info("Entering world!");
        return packet;
}

#endif
