#ifndef L2AUTH_GAME_ACTION_SELECT_CHARACTER_RESPONSE_H
#define L2AUTH_GAME_ACTION_SELECT_CHARACTER_RESPONSE_H

#include <core/l2_packet.h>
#include <game/client.h>
#include <game/entity/character.h>

l2_packet* game_action_select_character_response
(
        struct GameClient* client,
        struct GameEntityCharacter* character
);

#endif
