#ifndef L2AUTH_GAME_HANDLER_SELECT_CHARACTER_C
#define L2AUTH_GAME_HANDLER_SELECT_CHARACTER_C

#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/packet/player_auth_request.h>
#include <game/packet/char_selected.h>
#include <game/handler/select_character.h>

l2_packet* game_handler_select_character(l2_raw_packet* request, int playOK1)
{
        l2_packet* response = game_packet_char_selected(playOK1);
        return response;
}

#endif
