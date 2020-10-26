#include <assert.h>
#include <core/l2_string.h>
#include <core/l2_raw_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "../enter_world/next_handler.h"
#include "../../world_state.h"
#include "response.h"
#include "handler.h"

void say_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;
        l2_raw_packet *packet = request->packet;

        l2_string *message = NULL;
        l2_packet *response = NULL;

        message = (l2_string *)l2_packet_content(packet);
        response = say_response(client, message);

        // client_encrypt_packet(client, response);
        // client_queue_response(client, response);
        world_state_client_say(request->world_state, client, "Hi", 3);

        client_update_request_handler(client, &enter_world_next_handler);

        client_free_mem(client, response);
}
