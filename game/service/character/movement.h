#ifndef L2AUTH_GAME_SERVICE_CHARACTER_MOVEMENT_H
#define L2AUTH_GAME_SERVICE_CHARACTER_MOVEMENT_H

#include <game/entity/character.h>
#include <game/entity/location.h>

void game_service_character_movement_new_target
(
        struct GameEntityCharacter* character,
        struct GameEntityLocation* target
);

void game_service_character_movement_validate_and_update
(
        struct GameEntityCharacter* character,
        struct GameEntityLocation* location
);

#endif
