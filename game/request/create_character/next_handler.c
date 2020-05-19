#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/auth_login/handler.h>
#include "next_handler.h"

void game_request_create_character_next_handler(struct GameRequest* request)
{
        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        switch(packet_type) {
        case GAME_REQUEST_TYPE_AUTH_REQUEST:
                game_request_auth_login_handler(request);
                break;
        default:
                log_fatal("Invalid next state from create character");
                break;
        }
}
