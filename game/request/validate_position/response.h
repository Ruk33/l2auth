#ifndef L2AUTH_GAME_REQUEST_VALIDATE_POSITION_RESPONSE_H
#define L2AUTH_GAME_REQUEST_VALIDATE_POSITION_RESPONSE_H

#include <core/l2_packet.h>
#include <game/client.h>
#include <game/entity/vec3.h>

l2_packet* game_request_validate_position_response
(
        struct GameClient* client,
        struct Vec3 location,
        int heading
);

#endif
