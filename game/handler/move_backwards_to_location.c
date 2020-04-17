#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <game/dto/location.h>
#include <game/packet/char_move_to_loc.h>
#include <game/service/char_movement.h>
#include <game/handler/encrypt.h>
#include <game/handler/move_backwards_to_location.h>

void game_handler_move_backwards_to_location
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);
        assert(encrypt_key);

        struct GameDtoLocation* prev_location;
        struct GameDtoLocation new_location;

        l2_packet* response;

        prev_location = &l2_client_get_char(client)->current_location;

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

        game_service_char_movement_new_target(
                l2_client_get_char(client),
                &new_location
        );

        response = game_packet_char_move_to_loc(
                client,
                *prev_location,
                new_location
        );

        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );
}
