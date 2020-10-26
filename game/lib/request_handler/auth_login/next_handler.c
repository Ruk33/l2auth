#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "../type.h"
#include "../select_character/handler.h"
#include "next_handler.h"

void auth_login_next_handler(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        l2_packet_type type = l2_packet_get_type(packet);

        switch (type)
        {
        case REQUEST_TYPE_NEW_CHAR:
                log_warn("New char packet not implemented yet");
                break;
        case REQUEST_TYPE_SELECTED_CHAR:
                // select_character_handler(client, packet);
                break;
        default:
                log_fatal("Invalid state to be handled by auth_login_next_handler");
                break;
        }
}
