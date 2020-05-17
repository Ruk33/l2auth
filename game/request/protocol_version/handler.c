#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_packet.h>
#include <game/server.h>
#include <game/client.h>
#include <game/request.h>
#include "response.h"
#include "handler.h"

void game_request_protocol_version_handler(struct GameRequest* request)
{
        assert(request);

        struct GameClient* client = request->conn->client;
        unsigned char* content = l2_packet_content(request->packet);
        l2_packet* response = game_request_protocol_version_response(client);
        int protocol = 0;

        log_info("Handling protocol version packet");

        byte_reader_cpy_int_n_mv(content, &protocol);
        
        log_info("Protocol version %d", protocol);
        log_info("Returning crypt init");

        game_client_send_packet(client, response);
}
