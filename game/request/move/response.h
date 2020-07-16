#ifndef L2AUTH_GAME_REQUEST_MOVE_RESPONSE_H
#define L2AUTH_GAME_REQUEST_MOVE_RESPONSE_H

#include <core/l2_packet.h>
#include <game/client.h>
#include <game/entity/vec3.h>

l2_packet* game_request_move_response
(
        struct GameClient* client,
        struct Vec3 prev_location,
        struct Vec3 new_location
);

#endif
