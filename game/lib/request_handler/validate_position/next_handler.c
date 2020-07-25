#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "../move/next_handler.h"
#include "next_handler.h"

void validate_position_next_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);

        switch (type) {
        default:
                log_info("Delegating to move_next_handler");
                move_next_handler(client, packet);
                break;
        }
}
