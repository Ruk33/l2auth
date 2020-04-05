#ifndef L2AUTH_GAME_HANDLER_CREATE_CHARACTER_C
#define L2AUTH_GAME_HANDLER_CREATE_CHARACTER_C

#include <log/log.h>
#include <core/l2_raw_packet.c>
#include <core/l2_packet.c>
#include <game/packet/char_create_ok.c>

l2_packet* game_handler_create_character(l2_raw_packet* request)
{
        l2_packet* response = game_packet_char_create_ok();
        log_info("Creating new character");
        return response;
}

#endif
