#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../dto/player.h"
#include "../../dto/vec3.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *move_response
(struct Client *client, struct Vec3 prev_location, struct Vec3 new_location)
{
        assert(client);

        l2_packet_type type = 0x01;
        l2_packet *response = NULL;
        
        struct Player* player = NULL;
        int obj_id = 0;

        size_t buf_size = 0;
        byte_builder *buf = NULL;
        byte_builder *buffer = NULL;

        player = client_player(client);
        obj_id = player->character.id;

        buf_size = byte_builder_calculate_size(
                sizeof(obj_id) +
                sizeof(new_location) +
                sizeof(prev_location)
        );
        buf = client_alloc_mem(client, buf_size);
        buffer = byte_builder_init(buf, buf_size);

        log_info("Char move to loc -> obj id %d", obj_id);
        log_info("new x: %d, new y: %d, new z: %d", new_location.x, new_location.y, new_location.z);
        log_info("prev x: %d, prev y: %d, prev z: %d", prev_location.x, prev_location.y, prev_location.z);

        byte_builder_append_int(buffer, &obj_id);
        byte_builder_append_int(buffer, &new_location.x);
        byte_builder_append_int(buffer, &new_location.y);
        byte_builder_append_int(buffer, &new_location.z);
        byte_builder_append_int(buffer, &prev_location.x);
        byte_builder_append_int(buffer, &prev_location.y);
        byte_builder_append_int(buffer, &prev_location.z);

        response = packet_builder_new(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );

        client_free_mem(client, player);
        client_free_mem(client, buf);

        return response;
}
