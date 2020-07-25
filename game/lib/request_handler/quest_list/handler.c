#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../client.h"
#include "../type.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void quest_list_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet *response = quest_list_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, quest_list_next_handler);

        client_free_mem(client, response);
}
