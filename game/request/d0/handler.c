#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <game/request.h>
#include <game/client.h>
#include <game/service/crypt/packet/encrypt.h>
#include "next_handler.h"
#include "handler.h"
#include "response.h"

void game_request_d0_handler(struct GameRequest* request)
{
        assert(request);
        assert(request->packet);

        // unsigned char* encrypt_key = request->conn->encrypt_key;
        // struct GameClient* client = request->conn->client;
        // l2_packet* response = game_request_d0_response(client);
        unsigned char* content = l2_packet_content(request->packet);
        unsigned short id = 0;

        byte_reader_cpy_int_n_mv(content, &id);
        log_info("D0 ID: %d", id);

        switch (id) {
        case 8:
                log_info("Requesting Manor List");
                /*game_client_send_packet(
                        client, 
                        game_service_crypt_packet_encrypt(response, encrypt_key)
                );*/
                assert(request->conn);
                request->conn->handler = game_request_d0_next_handler;
                break;
        default:
                log_fatal("Unknown id to handle from packet D0");
                break;
        }
}
