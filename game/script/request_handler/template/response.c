#include <assert.h>
#include <stdlib.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *PACKET_LOWERCASE_NAME_response
(struct Client *client)
{
        assert(client);

        l2_packet_type type = 0x00;
        l2_packet *response = NULL;

        size_t buf_size = 0;
        byte_builder *buf = NULL;
        byte_builder *buffer = NULL;

        buf_size = byte_builder_calculate_size(0);
        buf = client_alloc_mem(client, buf_size);
        buffer = byte_builder_init(buf, buf_size);

        response = packet_builder_new(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );

        client_free_mem(client, buf);

        return response;
}
