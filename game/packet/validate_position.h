#ifndef L2AUTH_LOGIN_GAME_PACKET_VALIDATE_POSITION_H
#define L2AUTH_LOGIN_GAME_PACKET_VALIDATE_POSITION_H

#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <game/dto/location.h>

l2_packet* game_packet_validate_position
(
        struct L2Client* client,
        struct GameDtoLocation location,
        int heading
);

#endif
