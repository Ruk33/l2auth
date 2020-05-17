#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <game/server.h>
#include <game/client.h>
#include <game/entity/location.h>
#include <game/request/character_info/response.h>
#include <game/service/character/movement.h>
#include <game/service/crypt/packet/encrypt.h>
#include "response.h"
#include "handler.h"

void game_request_validate_position_handler
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

        struct GameEntityLocation location;
        int heading = 0;
        l2_packet* response;

        memcpy(&location.x, request + 3, sizeof(location.x));
        memcpy(&location.y, request + 3 + sizeof(location.x), sizeof(location.y));
        memcpy(&location.z, request + 3 + sizeof(location.x) + sizeof(location.y), sizeof(location.z));
        memcpy(&heading, request + 3 + sizeof(location.x) + sizeof(location.y) + sizeof(location.z), sizeof(heading));

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
}
