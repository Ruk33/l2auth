#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/move/next_handler.h>
#include "next_handler.h"

void game_request_validate_position_next_handler(struct GameRequest* request)
{
        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        switch(packet_type) {
        default:
                log_info("Delegating action to move from validate position");
                game_request_move_next_handler(request);
                break;
        }
}
