#include <assert.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "../type.h"
#include "../../world_state.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void enter_world_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;

        l2_packet *response = NULL;

        response = enter_world_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        world_state_client_info(request->world_state, client);

        client_update_request_handler(client, &enter_world_next_handler);

        client_free_mem(client, response);
}
