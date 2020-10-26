#include <assert.h>
#include <core/l2_raw_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void new_char_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;

        l2_packet *response = NULL;

        response = new_char_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, &new_char_next_handler);

        client_free_mem(client, response);
}
