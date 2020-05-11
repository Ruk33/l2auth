#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/service/crypt/packet/encrypt.h>
#include "response.h"
#include "handler.h"

void game_action_new_character_handler
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
        l2_packet* response = game_action_new_character_response(client);
        log_info("Handling new character");
        l2_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );
}
