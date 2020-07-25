#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_raw_packet.h>
#include "../../client.h"
#include "../type.h"
#include "../move/handler.h"
#include "../action/handler.h"
#include "../restart/handler.h"
#include "../say/handler.h"
#include "next_handler.h"

void enter_world_next_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);

        switch (type) {
        case REQUEST_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                move_handler(client, packet);
                break;
        case REQUEST_TYPE_ACTION:
                action_handler(client, packet);
                break;
        case REQUEST_TYPE_RESTART:
                restart_handler(client, packet);
                break;
        case REQUEST_TYPE_SAY:
                say_handler(client, packet);
                break;
        default:
                log_fatal("Invalid state to be handled by enter_world_next_handler");
                break;
        }
}
