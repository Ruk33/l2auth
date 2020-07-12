#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/client.h>
#include <game/service/crypt/packet/encrypt.h>
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void game_request_new_character_handler(struct GameRequest* request)
{
        assert(request);
        assert(request->conn);
        assert(request->conn->encrypt_key);
        assert(request->conn->client);

        unsigned char* encrypt_key = request->conn->encrypt_key;
        struct GameClient* client = request->conn->client;
        l2_packet* response = game_request_new_character_response(client);

        log_info("Handling new character");
        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        request->conn->handler = game_request_new_character_next_handler;
}
