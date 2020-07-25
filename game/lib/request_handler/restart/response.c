#include <core/l2_packet.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *restart_response(struct Client *client)
{
        l2_packet_type type = 0x5f;
        unsigned int content = 0x01;

        return packet_builder_new(
                client,
                type,
                (unsigned char *) &content,
                (unsigned short) sizeof(content)
        );
}
