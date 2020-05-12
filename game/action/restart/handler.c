#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/character/save.h>
#include <game/service/crypt/packet/encrypt.h>
#include <game/action/auth_login/handler.h>
#include "response.h"
#include "handler.h"

void game_action_restart_handler
(
        struct GameServer* server,
        struct GameClient* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);
        assert(encrypt_key);

        l2_packet* response = game_action_restart_response(client);
        log_info("Restarting");

        game_service_character_save(game_client_get_char(client));

        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        game_action_auth_login_handler(
                server,
                client,
                request,
                encrypt_key
        );
}
