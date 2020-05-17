#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/entity/location.h>
#include <game/request.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/character/movement.h>
#include <game/service/crypt/packet/encrypt.h>
#include "response.h"
#include "handler.h"

void game_request_move_handler(struct GameRequest* request)
{
        assert(request);

        struct GameServer* server = request->conn->server;
        struct GameClient* client = request->conn->client;

        unsigned char* content = l2_packet_content(request->packet);

        struct GameEntityLocation* prev_location;
        struct GameEntityLocation new_location;

        l2_packet* response;

        prev_location = &game_client_get_char(client)->current_location;

        content = byte_reader_cpy_int_n_mv(content, &new_location.x);
        content = byte_reader_cpy_int_n_mv(content, &new_location.y);
        content = byte_reader_cpy_int_n_mv(content, &new_location.z);

        log_info(
                "Trying to move character from x: %d, y: %d, z: %d to x: %d, y: %d, z: %d",
                prev_location->x,
                prev_location->y,
                prev_location->z,
                new_location.x,
                new_location.y,
                new_location.z
        );

        game_service_character_movement_new_target(
                game_client_get_char(client),
                &new_location
        );

        response = game_request_move_response(
                client,
                *prev_location,
                new_location
        );

       game_server_broadcast_packet_to_clients(server, response);
}
