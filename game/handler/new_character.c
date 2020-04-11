#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/packet/char_template.h>
#include <game/handler/encrypt.h>
#include <game/handler/new_character.h>

void game_handler_new_character
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
        l2_packet* response = game_packet_char_template(client);
        log_info("Handling new character");
        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );
}
