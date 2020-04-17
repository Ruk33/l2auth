#ifndef L2AUTH_GAME_SERVICE_CHAR_MOVEMENT_H
#define L2AUTH_GAME_SERVICE_CHAR_MOVEMENT_H

#include <game/dto/char.h>
#include <game/dto/location.h>

void game_service_char_movement_new_target
(
        struct GameDtoChar* character,
        struct GameDtoLocation* target
);

void game_service_char_movement_validate_and_update
(
        struct GameDtoChar* character,
        struct GameDtoLocation* location
);

#endif
