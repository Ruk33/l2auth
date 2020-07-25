#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../client.h"
#include "../quest_list/handler.h"
#include "../type.h"
#include "response.h"
#include "handler.h"

void d0_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);
        l2_packet *response;

        if (type != REQUEST_TYPE_REQUEST_AUTO_SS_BSPS) {
                log_fatal("Invalid request to be handled by D0");
                return;
        }

        response = d0_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, quest_list_handler);

        client_free_mem(client, response);
}
