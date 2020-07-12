#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/create_character/handler.h>
#include <game/request/auth_login/next_handler.h>
#include "next_handler.h"

void* game_request_new_character_next_handler(void* raw_request)
{
        assert(raw_request);

        struct GameRequest* request = (struct GameRequest*) raw_request;
        assert(request->packet);

        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        switch(packet_type) {
        case  GAME_REQUEST_TYPE_CREATE_CHAR:
                game_request_create_character_handler(request);
                break;
        default:
                log_info("Delegating action to auth login from new character");
                game_request_auth_login_next_handler(request);
                break;
        }

        return NULL;
}
