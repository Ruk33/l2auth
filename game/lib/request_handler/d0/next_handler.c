#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "../type.h"
#include "../quest_list/handler.h"
#include "next_handler.h"

void d0_next_handler(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        l2_packet_type type = l2_packet_get_type(packet);

        switch (type)
        {
        case REQUEST_TYPE_REQUEST_QUEST_LIST:
                // quest_list_handler(client, packet);
                break;
        default:
                log_fatal("Invalid state to be handled by d0_next_handler");
                break;
        }
}
