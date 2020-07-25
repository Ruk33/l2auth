#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "../type.h"
#include "../auth_login/handler.h"
#include "next_handler.h"

void protocol_version_next_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);

        switch (type) {
        case REQUEST_TYPE_AUTH_REQUEST:
                auth_login_handler(client, packet);
                break;
        default:
                log_fatal("Invalid state to be handled by protocol_version_next_handler");
                break;
        }
}
