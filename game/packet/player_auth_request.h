#ifndef L2AUTH_LOGIN_GAME_PACKET_PLAYER_AUTH_REQUEST_H
#define L2AUTH_LOGIN_GAME_PACKET_PLAYER_AUTH_REQUEST_H

#include <stdlib.h>
#include <core/l2_packet.h>
#include <login/dto/session_key.h>

l2_packet* game_packet_player_auth_request
(
        char* account,
        size_t account_length,
        struct LoginDtoSessionKey* key
);

#endif
