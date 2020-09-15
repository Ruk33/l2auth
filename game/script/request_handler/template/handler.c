#include <assert.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void PACKET_LOWERCASE_NAME_handler
(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        l2_packet *response = NULL;

        response = PACKET_LOWERCASE_NAME_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, &PACKET_LOWERCASE_NAME_next_handler);

        client_free_mem(client, response);
}
