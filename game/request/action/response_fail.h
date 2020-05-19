#ifndef L2AUTH_GAME_REQUEST_ACTION_FAIL_RESPONSE_H
#define L2AUTH_GAME_REQUEST_ACTION_FAIL_RESPONSE_H

#include <core/l2_packet.h>
#include <game/client.h>

l2_packet* game_request_action_fail_response(struct GameClient* client);

#endif