#include <log/log.h>
#include <core/byte_builder.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <game/dto/char.h>
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
        struct GameDtoChar* character = l2_client_get_char(client);
        int obj_id = character->char_id;

        log_fatal("char move to loc -> obj id %d", obj_id);
        log_fatal("new x: %d, new y: %d, new z: %d", new_location.x, new_location.y, new_location.z);
        log_fatal("prev x: %d, prev y: %d, prev z: %d", prev_location.x, prev_location.y, prev_location.z);

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
