#include <assert.h>
#include <core/l2_raw_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "../../entity/player.h"
#include "request.h"
#include "handler.h"

void create_char_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;
        l2_raw_packet *packet = request->packet;
        struct CreateCharRequest *char_create_request = NULL;

        char_create_request = create_char_request(client, packet);

        player_create(client, char_create_request);

        client_free_mem(client, char_create_request);
}
