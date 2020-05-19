#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_raw_packet.h>
#include <game/request.h>
#include <game/server.h>
#include <game/client.h>
#include <game/entity/location.h>
#include <game/request/character_info/response.h>
#include <game/service/character/movement.h>
#include <game/service/crypt/packet/encrypt.h>
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void game_request_validate_position_handler(struct GameRequest* request)
{
        assert(request);

        struct GameClient* server = request->conn->server;
        struct GameClient* client = request->conn->client;
        unsigned char* content = l2_packet_content(request->packet);
        int heading = 0;
        struct GameEntityLocation location;
        l2_packet* response;

        content = byte_reader_cpy_int_n_mv(content, &location.x);
        content = byte_reader_cpy_int_n_mv(content, &location.y);
        content = byte_reader_cpy_int_n_mv(content, &location.z);
        content = byte_reader_cpy_int_n_mv(content, &heading);

        log_info(
                "Validating position - x: %d, y: %d, z: %d",
                location.x,
                location.y,
                location.z
        );

        game_service_character_movement_validate_and_update(
                game_client_get_char(client),
                &location
        );

        response = game_request_validate_position_response(
                client,
                game_client_get_char(client)->current_location,
                heading
        );
        game_server_broadcast_packet_to_clients(server, response);

        // (franco.montenegro) Refactor, validate shouldn't
        // send this response, there should be a handler
        // managing this response
        response = game_request_character_info_response(client);
        game_server_broadcast_packet(server, client, response);

        request->conn->handler = game_request_validate_position_next_handler;
}
