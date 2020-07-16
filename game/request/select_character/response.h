#ifndef L2AUTH_GAME_REQUEST_SELECT_CHARACTER_RESPONSE_H
#define L2AUTH_GAME_REQUEST_SELECT_CHARACTER_RESPONSE_H

#include <core/l2_packet.h>
#include <game/client.h>
#include <game/entity/player.h>

l2_packet* game_request_select_character_response
(
        struct GameClient* client,
        struct Player* player
);

#endif
