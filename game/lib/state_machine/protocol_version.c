#include <request.h>
#include <server_packet/protocol_version.h>
#include <client_packet/type.h>
#include <util/session_crypt.h>
#include "protocol_version.h"

#define packet_size PACKET_SAFE_FULL_SIZE(server_packet_protocol_version_t)

/**
 * When the user is switching from login to game server
 * this is the packet to be handled.
 */
static void protocol_version(request_t *req)
{
        packet response[packet_size] = { 0 };

        assert_valid_request(req);

        server_packet_protocol_version(response);
        request_send_response(req, response, packet_get_size(response));
        util_session_update_state(req->storage, req->socket, AUTH_REQUEST);
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
                printf("Packet %02X can't be handled by state_machine_protocol_version.\n",
                       type);
                break;
        }
}
