#include <assert.h>
#include <stdlib.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *show_map_response(struct Client *client, int map_id)
{
        assert(client);

        l2_packet_type type = 0x9d;

        return packet_builder_new(
            client,
            type,
            (unsigned char *)&map_id,
            (unsigned short)sizeof(map_id));
}
