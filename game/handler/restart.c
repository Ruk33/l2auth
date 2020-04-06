#ifndef L2AUTH_GAME_HANDLER_RESTART_C
#define L2AUTH_GAME_HANDLER_RESTART_C

#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/packet/restart.h>
#include <game/handler/restart.h>

l2_packet* game_handler_restart(l2_raw_packet* request)
{
        l2_packet* packet = game_packet_restart();
        log_info("Restarting");
        return packet;
}

#endif
