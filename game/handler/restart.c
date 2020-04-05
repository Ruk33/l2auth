#ifndef L2AUTH_GAME_HANDLER_RESTART_C
#define L2AUTH_GAME_HANDLER_RESTART_C

#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.c>
#include <core/l2_packet.c>
#include <game/packet/restart.c>

l2_packet* game_handler_restart(l2_raw_packet* request)
{
        l2_packet* packet = game_packet_restart();
        log_info("Restarting");
        return packet;
}

#endif
