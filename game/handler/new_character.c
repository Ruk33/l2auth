#ifndef L2AUTH_GAME_HANDLER_NEW_CHARACTER_C
#define L2AUTH_GAME_HANDLER_NEW_CHARACTER_C

#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.c>
#include <core/l2_packet.c>
#include <game/packet/char_template.c>

l2_packet* game_handler_new_character(l2_raw_packet* request)
{
        l2_packet* response = game_packet_char_template();
        log_info("Handling new character");
        return response;
}

#endif
