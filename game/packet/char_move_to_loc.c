#include <core/byte_builder.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <game/dto/location.h>
#include <game/packet/char_move_to_loc.h>

l2_packet* game_packet_char_move_to_loc
(
        struct L2Client* client,
        struct GameDtoLocation prev_location,
        struct GameDtoLocation new_location
)
{
        l2_packet_type type = 0x01;
        int obj_id = 0x01;
        byte_builder* buffer = l2_client_byte_builder(
                client,
                sizeof(obj_id) +
                sizeof(new_location) +
                sizeof(prev_location)
        );

        byte_builder_append(buffer, (unsigned char *) &obj_id, sizeof(obj_id));

        byte_builder_append(
                buffer,
                (unsigned char *) &(new_location.x),
                sizeof(new_location.x)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &(new_location.y),
                sizeof(new_location.y)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &(new_location.z),
                sizeof(new_location.z)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &(prev_location.x),
                sizeof(prev_location.x)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &(prev_location.y),
                sizeof(prev_location.y)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &(prev_location.z),
                sizeof(prev_location.z)
        );

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
