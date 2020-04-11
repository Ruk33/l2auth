#ifndef L2AUTH_GAME_HANDLER_SELECT_CHARACTER_C
#define L2AUTH_GAME_HANDLER_SELECT_CHARACTER_C

#include <assert.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <game/packet/char_selected.h>
#include <game/handler/encrypt.h>
#include <game/handler/select_character.h>

void game_handler_select_character
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);
        assert(encrypt_key);

        l2_packet* response = game_packet_char_selected(client);
        
        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );
}

#endif
