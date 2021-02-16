#include <request.h>
#include <server_packet/protocol_version.h>
#include <client_packet/type.h>
#include "protocol_version.h"

/**
 * When the user is switching from login to game server
 * this is the packet to be handled.
 */
static void protocol_version(request_t *request)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_protocol_version_t)] = {0};

        assert_valid_request(request);

        server_packet_protocol_version(response);
        request->host->send_response(request->session->socket, response, (size_t) packet_get_size(response));

        session_update_state(request->session, AUTH_REQUEST);
}

void state_machine_protocol_version(request_t *request)
{
        packet_type type = 0;

        assert_valid_request(request);

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_PROTOCOL_VERSION:
                protocol_version(request);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_protocol_version.\n", type);
                break;
        }
}
