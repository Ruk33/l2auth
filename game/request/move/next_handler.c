#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/validate_position/handler.h>
#include <game/request/enter_world/next_handler.h>
#include "next_handler.h"

void game_request_move_next_handler(struct GameRequest* request)
{
        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        switch(packet_type) {
        case  GAME_REQUEST_TYPE_VALIDATE_POS:
                game_request_validate_position_handler(request);
                break;
        default:
                log_info("Delegating action to enter world from move");
                game_request_enter_world_next_handler(request);
                break;
        }
}
