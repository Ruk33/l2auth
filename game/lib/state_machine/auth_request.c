#include "../client_request/auth_request.h"
#include "../client_packet/type.h"
#include "auth_request.h"

void state_machine_auth_request(request_t *request)
{
        assert_valid_request(request);

        packet_type type = 0;

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_AUTH_REQUEST:
                client_request_auth_request(request->session->socket, request->packet, &request->storage->character_storage, request->session, request->host->send_response);
                session_update_state(request->session, CHARACTER_SELECTION);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_auth_request.\n", type);
                break;
        }
}
