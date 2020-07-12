#include <assert.h>
#include <core/l2_packet.h>
#include <game/client.h>
#include "response.h"

l2_packet* game_request_restart_response(struct GameClient* client)
{
        assert(client);

        l2_packet_type type = 0x5f;
        unsigned int content = 0x01;

        return game_client_create_packet(
                client,
                type,
                (unsigned char *) &content,
                (unsigned short) sizeof(content)
        );
}
