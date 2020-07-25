#include <core/l2_packet.h>
#include "../../client.h"
#include "../../packet_builder.h"

l2_packet *protocol_version_response(struct Client *client)
{
        l2_packet_type type = 0x00;

        unsigned char content[] = {
                0x01,
                // crypt key
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        return packet_builder_new(client, type, content, sizeof(content));
}
