#include <request.h>
#include <client_request/protocol_version.h>
#include <client_packet/type.h>
#include "protocol_version.h"

void state_machine_protocol_version(request_t *request)
{
        assert_valid_request(request);

        packet_type type = 0;

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_PROTOCOL_VERSION:
                client_request_protocol_version(request->session->socket, request->host->send_response);
                session_update_state(request->session, AUTH_REQUEST);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_protocol_version.\n", type);
                break;
        }
}
