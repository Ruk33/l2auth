#include <stdlib.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../entity/pc.h"
#include "../../entity/vec3.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *move_response(struct Client *client)
{
        l2_packet_type type = 0x01;
        
        struct Pc* player = client_character(client);
        int obj_id = player->character.id;

        struct Vec3 prev_location;
        struct Vec3 new_location;

        size_t buf_size = sizeof(obj_id) + sizeof(new_location) + sizeof(prev_location);
        byte_builder *buf = game_client_byte_builder(client, buf_size);
        byte_builder *buffer = byte_builder_init(buf, buf_size);

        log_fatal("char move to loc -> obj id %d", obj_id);
        log_fatal("new x: %d, new y: %d, new z: %d", new_location.x, new_location.y, new_location.z);
        log_fatal("prev x: %d, prev y: %d, prev z: %d", prev_location.x, prev_location.y, prev_location.z);

        byte_builder_append(buffer, &obj_id, sizeof(obj_id));
        byte_builder_append(buffer, &new_location.x, sizeof(new_location.x));
        byte_builder_append(buffer, &new_location.y, sizeof(new_location.y));
        byte_builder_append(buffer, &new_location.z, sizeof(new_location.z));
        byte_builder_append(buffer, &prev_location.x, sizeof(prev_location.x));
        byte_builder_append(buffer, &prev_location.y, sizeof(prev_location.y));
        byte_builder_append(buffer, &prev_location.z, sizeof(prev_location.z));

        return packet_builder_new(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
