#include <assert.h>
#include <core/l2_packet.h>
#include <game/client.h>
#include "response.h"

l2_packet* game_request_quest_list_response(struct GameClient* client)
{
        assert(client);

        l2_packet_type type = 0x80;
        unsigned char content[] = {
                0,
                0,
                0,
                0
        };

        return game_client_create_packet(
                client,
                type,
                content,
                (unsigned short) sizeof(content)
        );
}
