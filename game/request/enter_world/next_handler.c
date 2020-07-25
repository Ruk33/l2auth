#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/move/handler.h>
#include <game/request/action/handler.h>
#include <game/request/restart/handler.h>
#include <game/request/say/handler.h>
#include "next_handler.h"

void* game_request_enter_world_next_handler(void* raw_request)
{
        assert(raw_request);

        struct GameRequest* request = (struct GameRequest*) raw_request;
        assert(request->packet);

        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        log_info("Handling from enter world next handler!");

        switch(packet_type) {
        case  GAME_REQUEST_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                game_request_move_handler(request);
                break;
        case GAME_REQUEST_TYPE_ACTION:
                game_request_action_handler(request);
                break;
        case  GAME_REQUEST_TYPE_RESTART:
                game_request_restart_handler(request);
                break;
        case  GAME_REQUEST_TYPE_SAY:
                game_request_say_handler(request);
                break;
        default:
                log_fatal("Invalid next state from enter world");
                break;
        }

        return NULL;
}
