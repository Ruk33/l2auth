#include <assert.h>
#include <stdlib.h>
#include <core/l2_string.h>
#include <core/l2_raw_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "../enter_world/next_handler.h"
#include "../../world_manager.h"
#include "response.h"
#include "handler.h"

void say_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;
        l2_raw_packet *packet = request->packet;

        l2_string *l2_message = NULL;
        char *message = NULL;
        size_t message_size = 0;

        l2_message = (l2_string *)l2_packet_content(packet);
        message_size = l2_string_len(l2_message) + 1;
        message = client_alloc_mem(client, message_size);
        l2_string_to_char(l2_message, message, message_size);

        world_manager_active_char_talk(request->world_state, client, message, message_size);

        client_update_request_handler(client, &enter_world_next_handler);

        client_free_mem(client, message);
}
