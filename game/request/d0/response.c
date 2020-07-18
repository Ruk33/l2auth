#include <assert.h>
#include <string.h>
#include <core/l2_packet.h>
#include <game/client.h>
#include "response.h"

l2_packet* game_request_d0_response(struct GameClient* client)
{
        assert(client);

        l2_packet_type type = 0xFE;
        unsigned char content[] = { 0, 0, 0, 0, 0, 0 };
        unsigned short unkown = 0x1B;
        unsigned int manor_size = 0;

        // memset(content, 0, sizeof(content));
        memcpy(content, &unkown, sizeof(unkown));
        memcpy(content + sizeof(unkown), &manor_size, sizeof(manor_size));

        return game_client_create_packet(client, type, content, sizeof(content));
}
