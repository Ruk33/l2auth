#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/enter_world/next_handler.h>
#include "next_handler.h"

void* game_request_say_next_handler(void* raw_request)
{
        assert(raw_request);

        struct GameRequest* request = (struct GameRequest*) raw_request;
        assert(request->packet);
        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        switch(packet_type) {
        default:
                log_info("Delegating action to enter world from say");
                game_request_enter_world_next_handler(request);
                break;
        }

        return NULL;
}
