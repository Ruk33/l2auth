#include <assert.h>
#include <log/log.h>
#include <game/request.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/crypt/packet/encrypt.h>
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void game_request_enter_world_handler(struct GameRequest* request)
{
        assert(request);

        unsigned char* encrypt_key = request->conn->encrypt_key;
        struct GameClient* client = request->conn->client;
        l2_packet* response = game_request_enter_world_response(client);
        log_info("Entering world!");

        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        request->conn->handler = game_request_enter_world_next_handler;
}
