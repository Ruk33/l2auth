#include <core/l2_packet.h>
#include <game/client.h>
#include "response.h"

l2_packet* game_request_protocol_version_response(struct GameClient* client)
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

        return game_client_create_packet(
                client,
                type,
                content,
                (unsigned short) sizeof(content)
        );
}
