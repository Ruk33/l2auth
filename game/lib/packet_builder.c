#include <assert.h>
#include <stdlib.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "client.h"
#include "packet_builder.h"

l2_packet *packet_builder_new
(
        struct Client *client,
        l2_packet_type type,
        unsigned char *buf,
        size_t buf_size
)
{
        assert(client);
        assert(buf);
        assert(buf_size > 0);

        l2_raw_packet_size packet_size = 0;
        l2_packet *packet = NULL;

        packet_size = l2_packet_calculate_size((unsigned short) buf_size);
        packet = client_alloc_mem(client, packet_size);

        l2_packet_init(packet, type, buf, (unsigned short) buf_size);

        return packet;
}
