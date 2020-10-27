#include <assert.h>
#include <core/l2_raw_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void logout_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;

        l2_packet *response = NULL;

        response = logout_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_free_mem(client, response);
}
