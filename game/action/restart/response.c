#include <core/l2_packet.h>
#include "response.h"

l2_packet* game_action_restart_response(struct L2Client* client)
{
        l2_packet_type type = 0x5f;
        unsigned int content = 0x01;

        return l2_client_create_packet(
                client,
                type,
                (unsigned char *) &content,
                (unsigned short) sizeof(content)
        );
}
