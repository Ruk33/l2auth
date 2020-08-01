#include <stdlib.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../dto/pc.h"
#include "../../dto/vec3.h"
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *validate_position_response(struct Client *client, struct Vec3 location, int heading)
{
        l2_packet_type type = 0x61;
        l2_packet *response;

        struct Pc *player = client_player(client);
        int char_id = player->character.id;

        size_t buf_size = byte_builder_calculate_size(
                sizeof(char_id) +
                sizeof(struct Vec3) +
                sizeof(heading)
        );
        byte_builder *buf = client_alloc_mem(client, buf_size);
        byte_builder *buffer = byte_builder_init(buf, buf_size);

        byte_builder_append(buffer, &char_id, sizeof(char_id));
        byte_builder_append(buffer, &location.x, sizeof(location.x));
        byte_builder_append(buffer, &location.y, sizeof(location.y));
        byte_builder_append(buffer, &location.z, sizeof(location.z));
        byte_builder_append(buffer, &heading, sizeof(heading));

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
