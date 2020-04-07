#ifndef L2AUTH_GAME_HANDLER_AUTH_LOGIN_H
#define L2AUTH_GAME_HANDLER_AUTH_LOGIN_H

#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <login/dto/session_key.h>

l2_packet* game_handler_auth_login
(
        l2_raw_packet* request,
        struct LoginDtoSessionKey* session_key
);

#endif
