#ifndef L2AUTH_GAME_HANDLER_CREATE_CHARACTER_C
#define L2AUTH_GAME_HANDLER_CREATE_CHARACTER_C

#include <log/log.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/packet/char_create_ok.h>
#include <game/handler/encrypt.h>
#include <game/handler/auth_login.h>
#include <game/handler/create_character.h>

void game_handler_create_character
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        l2_packet* response = game_packet_char_create_ok(client);
        log_info("Creating new character");
        
        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );

        game_handler_auth_login(server, client, request, encrypt_key);
}

#endif
