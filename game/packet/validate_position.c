#include <assert.h>
#include <core/byte_builder.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <game/dto/location.h>
#include <game/packet/validate_position.h>

l2_packet* game_packet_validate_position
(
        struct L2Client* client,
        struct GameDtoLocation location,
        int heading
)
{
        assert(client);

        l2_packet_type type = 0x61;
        int char_id = 0x01;
        byte_builder* buffer = l2_client_byte_builder(
                client,
                sizeof(char_id) +
                sizeof(struct GameDtoLocation) +
                sizeof(heading)
        );

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

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
