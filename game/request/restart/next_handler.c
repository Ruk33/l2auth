#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/request/type.h>
#include <game/request/auth_login/next_handler.h>
#include "next_handler.h"

void game_request_restart_next_handler(struct GameRequest* request)
{
        l2_packet* packet = request->packet;
        l2_packet_type packet_type = (l2_packet_type) (packet[2] & 0xff);

        switch(packet_type) {
        default:
                log_info("Delegating action to auth request from restart");
                game_request_auth_login_next_handler(request);
                break;
        }
}
