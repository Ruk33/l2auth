#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "response.h"
#include "handler.h"

void npc_info_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet *response = npc_info_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_free_mem(client, response);
}
