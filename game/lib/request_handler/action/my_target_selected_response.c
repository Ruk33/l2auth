#include <stdlib.h>
#include <assert.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "my_target_selected_response.h"

l2_packet *my_target_selected_response
(struct Client *client, int object_id, short color)
{
        assert(client);

        l2_packet_type type = 0xa6;
        l2_packet *response = NULL;

        size_t buf_size = 0;
        byte_builder *buf = NULL;
        byte_builder *buffer = NULL;

        buf_size = byte_builder_calculate_size(sizeof(object_id) + sizeof(color));
        buf = client_alloc_mem(client, buf_size);
        buffer = byte_builder_init(buf, buf_size);

        byte_builder_append_int(buffer, &object_id);
        byte_builder_append_short(buffer, &color);

        response = packet_builder_new(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );

        client_free_mem(client, buf);

        return response;
}