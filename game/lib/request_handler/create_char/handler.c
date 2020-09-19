#include <assert.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "../../entity/player.h"
#include "request.h"
#include "handler.h"

void create_char_handler
(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        struct CreateCharRequest *request = NULL;

        request = create_char_request(client, packet);

        player_create(client, request);

        client_free_mem(client, request);
}
