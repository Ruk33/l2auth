#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "../type.h"
#include "../validate_position/handler.h"
#include "../enter_world/next_handler.h"
#include "next_handler.h"

void move_next_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);

        log_info("Move next handler");
        log_info("Packet type %02X", type);
        log_info("Packet type should be %02X", REQUEST_TYPE_VALIDATE_POS);

        switch (type) {
        case REQUEST_TYPE_VALIDATE_POS:
                validate_position_handler(client, packet);
                break;
        default:
                log_info("Delegating to enter_world_next_handler");
                enter_world_next_handler(client, packet);
                break;
        }
}
