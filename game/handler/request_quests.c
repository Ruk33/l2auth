#ifndef L2AUTH_GAME_HANDLER_REQUEST_QUESTS_C
#define L2AUTH_GAME_HANDLER_REQUEST_QUESTS_C

#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.c>
#include <core/l2_packet.c>
#include <game/packet/quest_list.c>

l2_packet* game_handler_request_quests(l2_raw_packet* request)
{
        l2_packet* packet = game_packet_quest_list();
        log_info("Sending quests (empty obviously)");
        return packet;
}

#endif
