#include <core/l2_packet.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "action_fail_response.h"

l2_packet *action_fail_response(struct Client *client)
{
        l2_packet_type type = 0x25;
        unsigned char content[] = { 0, 0, 0, 0 };
        return packet_builder_new(client, type, content, sizeof(content));
}
