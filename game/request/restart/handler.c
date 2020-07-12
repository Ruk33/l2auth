#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/client.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/character/save.h>
#include <game/service/crypt/packet/encrypt.h>
#include <game/request/auth_login/handler.h>
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void game_request_restart_handler(struct GameRequest* request)
{
        assert(request);
        assert(request->conn);
        assert(request->conn->encrypt_key);
        assert(request->conn->client);

        unsigned char* encrypt_key = request->conn->encrypt_key;
        struct GameClient* client = request->conn->client;
        l2_packet* response = game_request_restart_response(client);
        log_info("Restarting");

        game_service_character_save(game_client_get_char(client));

        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        game_request_auth_login_handler(request);

        request->conn->handler = game_request_restart_next_handler;
}
