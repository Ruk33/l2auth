#ifndef L2AUTH_LOGIN_GAME_PACKET_CHAR_MOVE_TO_LOC_H
#define L2AUTH_LOGIN_GAME_PACKET_CHAR_MOVE_TO_LOC_H

#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <game/dto/location.h>

l2_packet* game_packet_char_move_to_loc
(
        struct L2Client* client,
        struct GameDtoLocation prev_location,
        struct GameDtoLocation new_location
);

#endif
