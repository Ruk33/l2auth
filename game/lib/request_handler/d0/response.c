#include <string.h>
#include <core/l2_packet.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *d0_response(struct Client *client)
{
        l2_packet_type type = 0xFE;
        unsigned char content[] = { 0, 0, 0, 0, 0, 0 };
        unsigned short unkown = 0x1B;
        unsigned int manor_size = 0;

        memcpy(content, &unkown, sizeof(unkown));
        memcpy(content + sizeof(unkown), &manor_size, sizeof(manor_size));

        return packet_builder_new(client, type, content, sizeof(content));
}
