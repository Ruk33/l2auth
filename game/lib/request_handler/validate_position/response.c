#include <assert.h>
#include <stdlib.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../dto/pc.h"
#include "../../dto/vec3.h"
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *validate_position_response
(struct Client *client, struct Vec3 location, int heading)
{
        assert(client);

        l2_packet_type type = 0x61;
        l2_packet *response = NULL;

        struct Pc *player = NULL;
        int char_id = 0;

        size_t buf_size = 0;
        byte_builder *buf = NULL;
        byte_builder *buffer = NULL;

        player = client_player(client);
        char_id = player->character.id;

        buf_size = byte_builder_calculate_size(
                sizeof(char_id) +
                sizeof(struct Vec3) +
                sizeof(heading)
        );
        buf = client_alloc_mem(client, buf_size);
        buffer = byte_builder_init(buf, buf_size);

        byte_builder_append_int(buffer, &char_id);
        byte_builder_append_int(buffer, &location.x);
        byte_builder_append_int(buffer, &location.y);
        byte_builder_append_int(buffer, &location.z);
        byte_builder_append_int(buffer, &heading);

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
