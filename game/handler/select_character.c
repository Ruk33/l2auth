#ifndef L2AUTH_GAME_HANDLER_SELECT_CHARACTER_C
#define L2AUTH_GAME_HANDLER_SELECT_CHARACTER_C

#include <core/l2_packet.c>
#include <game/packet/player_auth_request.c>
#include <game/packet/char_selected.c>

l2_packet* game_handler_select_character(l2_raw_packet* request)
{
        l2_packet* response = game_packet_char_selected(0);
        return response;
}

#endif
