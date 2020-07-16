#ifndef L2AUTH_GAME_SERVICE_CHARACTER_MOVEMENT_H
#define L2AUTH_GAME_SERVICE_CHARACTER_MOVEMENT_H

#include <game/entity/player.h>
#include <game/entity/vec3.h>

void game_service_character_movement_new_target
(
        struct Player* character,
        struct Vec3* target
);

void game_service_character_movement_validate_and_update
(
        struct Player* character,
        struct Vec3* location
);

#endif
