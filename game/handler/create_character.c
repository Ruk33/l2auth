#ifndef L2AUTH_GAME_HANDLER_CREATE_CHARACTER_C
#define L2AUTH_GAME_HANDLER_CREATE_CHARACTER_C

#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/packet/char_create_ok.h>
#include <game/handler/create_character.h>

l2_packet* game_handler_create_character(l2_raw_packet* request)
{
        l2_packet* response = game_packet_char_create_ok();
        log_info("Creating new character");
        return response;
}

#endif
