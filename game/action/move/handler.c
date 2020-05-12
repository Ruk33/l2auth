#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/entity/location.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/character/movement.h>
#include <game/service/crypt/packet/encrypt.h>
#include "response.h"
#include "handler.h"

void game_action_move_handler
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

        struct GameEntityLocation* prev_location;
        struct GameEntityLocation new_location;

        l2_packet* response;

        prev_location = &game_client_get_char(client)->current_location;

        memcpy(&new_location.x, request + 3, sizeof(new_location.x));
        memcpy(&new_location.y, request + 3 + sizeof(new_location.x), sizeof(new_location.y));
        memcpy(&new_location.z, request + 3 + sizeof(new_location.x) + sizeof(new_location.y), sizeof(new_location.z));

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

        response = game_action_move_response(
                client,
                *prev_location,
                new_location
        );

       game_server_broadcast_packet_to_clients(server, response);
}
