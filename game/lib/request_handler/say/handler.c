#include <core/l2_string.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "../enter_world/next_handler.h"
#include "response.h"
#include "handler.h"

void say_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_string *message = (l2_string *) l2_packet_content(packet);
        l2_packet *response = say_response(client, message);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, &enter_world_next_handler);

        client_free_mem(client, response);
}
