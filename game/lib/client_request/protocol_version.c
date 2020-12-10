#include "../headers.h"
#include "../server_packet/protocol_version.h"
#include "protocol_version.h"

void client_request_protocol_version(
        int client,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(send_response);

        packet response[SERVER_PACKET_PROTOCOL_VERSION_FULL_SIZE] = {0};

        server_packet_protocol_version(response);
        send_response(client, response, (size_t) packet_get_size(response));
}
