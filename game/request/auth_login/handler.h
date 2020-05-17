#ifndef L2AUTH_GAME_REQUEST_AUTH_LOGIN_HANDLER_H
#define L2AUTH_GAME_REQUEST_AUTH_LOGIN_HANDLER_H

#include <core/l2_raw_packet.h>
#include <core/session_key.h>
#include <game/server.h>
#include <game/client.h>

void game_request_auth_login_handler
(
        struct GameServer* server,
        struct GameClient* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
);

#endif
