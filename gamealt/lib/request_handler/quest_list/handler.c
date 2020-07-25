#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../client.h"
#include "../enter_world/handler.h"
#include "../type.h"
#include "response.h"
#include "handler.h"

void quest_list_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);
        l2_packet *response;

        if (type != REQUEST_TYPE_REQUEST_QUEST_LIST) {
                log_fatal("Invalid request to be handled by quest list");
                return;
        }

        response = quest_list_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, enter_world_handler);

        client_free_mem(client, response);
}
