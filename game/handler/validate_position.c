#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <game/dto/location.h>
#include <game/packet/validate_position.h>
#include <game/service/char_movement.h>
#include <game/handler/encrypt.h>
#include <game/handler/validate_position.h>

void game_handler_validate_position
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

        struct GameDtoLocation location;
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

        game_service_char_movement_validate_and_update(
                l2_client_get_char(client),
                &location
        );

        response = game_packet_validate_position(
                client,
                l2_client_get_char(client)->current_location,
                heading
        );

        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );
}
