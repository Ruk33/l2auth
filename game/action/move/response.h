#ifndef L2AUTH_GAME_ACTION_MOVE_RESPONSE_H
#define L2AUTH_GAME_ACTION_MOVE_RESPONSE_H

#include <core/l2_packet.h>
#include <game/client.h>
#include <game/entity/location.h>

l2_packet* game_action_move_response
(
        struct GameClient* client,
        struct GameEntityLocation prev_location,
        struct GameEntityLocation new_location
);

#endif
