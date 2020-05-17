#include <core/l2_packet.h>
#include <game/client.h>
#include "response_ok.h"

l2_packet* game_request_character_create_response_ok(struct GameClient* client)
{
        l2_packet_type type = 0x19;
        unsigned int content = 0x01;

        return game_client_create_packet(
                client,
                type,
                (unsigned char *) &content,
                (unsigned short) sizeof(content)
        );
}
