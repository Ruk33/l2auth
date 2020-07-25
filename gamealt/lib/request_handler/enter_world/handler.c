#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../client.h"
#include "../type.h"
#include "../restart/handler.h"
#include "response.h"
#include "handler.h"

void enter_world_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);
        l2_packet *response;

        if (type != REQUEST_TYPE_ENTER_WORLD) {
                log_fatal("Invalid request to be handled by enter world");
                return;
        }

        response = enter_world_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, restart_handler);

        client_free_mem(client, response);
}
