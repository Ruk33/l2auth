#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/quest_list/handler.h>
#include "next_handler.h"

void* game_request_d0_next_handler(void* raw_request)
{
        assert(raw_request);

        struct GameRequest* request = (struct GameRequest*) raw_request;
        assert(request->packet);

        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        switch(packet_type) {
        case  GAME_REQUEST_TYPE_REQUEST_QUEST_LIST:
                game_request_quest_list_handler(request);
                break;
        default:
                log_fatal("Invalid next state from d0");
                break;
        }

        return NULL;
}
