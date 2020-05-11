#include <core/l2_packet.h>
#include <core/l2_client.h>
#include "response_ok.h"

l2_packet* game_action_character_create_response_ok(struct L2Client* client)
{
        l2_packet_type type = 0x19;
        unsigned int content = 0x01;

        return l2_client_create_packet(
                client,
                type,
                (unsigned char *) &content,
                (unsigned short) sizeof(content)
        );
}
