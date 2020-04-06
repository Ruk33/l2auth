#ifndef L2AUTH_GAME_HANDLER_AUTH_LOGIN_C
#define L2AUTH_GAME_HANDLER_AUTH_LOGIN_C

#include <string.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <login/dto/session_key.h>
#include <game/packet/player_auth_request.h>
#include <game/packet/char_list.h>
#include <game/handler/auth_login.h>

void game_handler_cpy_login_name(l2_raw_packet* packet, char* dest)
{
        int z = 0;
        for (int i = 3; packet[i]; i+=2) {
                dest[z] = packet[i];
                dest[z+1] = 0;
                z++;
        }
}

l2_packet* game_handler_auth_login
(
        l2_raw_packet* request,
        struct LoginDtoSessionKey* session_key
)
{
        size_t login_name_size = 0;

        game_handler_cpy_login_name(request, session_key->login_name);
        log_info("Login name %s", session_key->login_name);

        login_name_size = strlen(session_key->login_name) * 2 + 2;

        memcpy(&session_key->playOK2, request + 3 + login_name_size, sizeof(int));
        memcpy(&session_key->playOK1, request + 3 + login_name_size + sizeof(int), sizeof(int));
        memcpy(&session_key->loginOK1, request + 3 + login_name_size + sizeof(int) * 2, sizeof(int));
        memcpy(&session_key->loginOK2, request + 3 + login_name_size + sizeof(int) * 3, sizeof(int));

        log_info("Session key - Play OK 1 %d", session_key->playOK1);
        log_info("Session key - Play OK 2 %d", session_key->playOK2);
        log_info("Session key - Login OK 1 %d", session_key->loginOK1);
        log_info("Session key - Login OK 2 %d", session_key->loginOK2);

        l2_packet* response = game_packet_char_list(0);
        return response;
}

#endif
