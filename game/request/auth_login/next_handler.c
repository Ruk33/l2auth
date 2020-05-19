#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/new_character/handler.h>
#include <game/request/select_character/handler.h>
#include "next_handler.h"

void game_request_auth_login_next_handler(struct GameRequest* request)
{
        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        switch(packet_type) {
        case  GAME_REQUEST_TYPE_NEW_CHAR:
                game_request_new_character_handler(request);
                break;
        case  GAME_REQUEST_TYPE_SELECTED_CHAR:
                game_request_select_character_handler(request);
                break;
        default:
                log_fatal("Invalid next state from auth login");
                break;
        }
}
