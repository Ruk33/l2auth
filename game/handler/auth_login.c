#ifndef L2AUTH_GAME_HANDLER_AUTH_LOGIN_C
#define L2AUTH_GAME_HANDLER_AUTH_LOGIN_C

#include <core/l2_packet.c>
#include <game/packet/player_auth_request.c>
#include <game/packet/char_list.c>

l2_packet* game_handler_auth_login(l2_raw_packet* request)
{

        l2_packet* response = game_packet_char_list(0);
        return response;
}

#endif
