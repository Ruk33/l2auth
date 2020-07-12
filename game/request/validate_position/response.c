#include <assert.h>
#include <core/byte_builder.h>
#include <core/l2_packet.h>
#include <game/client.h>
#include <game/entity/character.h>
#include <game/entity/location.h>
#include "response.h"

l2_packet* game_request_validate_position_response
(
        struct GameClient* client,
        struct GameEntityLocation location,
        int heading
)
{
        assert(client);

        l2_packet_type type = 0x61;
        struct GameEntityCharacter* character = game_client_get_char(client);
        int char_id = character->char_id;

        byte_builder* buffer = game_client_byte_builder(
                client,
                sizeof(char_id) +
                sizeof(struct GameEntityLocation) +
                sizeof(heading)
        );
        assert(buffer);

        byte_builder_append(buffer, (unsigned char *) &char_id, sizeof(char_id));

        byte_builder_append(
                buffer,
                (unsigned char *) &(location.x),
                sizeof(location.x)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &(location.y),
                sizeof(location.y)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &(location.z),
                sizeof(location.z)
        );

        byte_builder_append(buffer, (unsigned char *) &heading, sizeof(heading));

        return game_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
