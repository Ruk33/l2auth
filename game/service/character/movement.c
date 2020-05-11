#include <assert.h>
#include <math.h>
#include <game/entity/character.h>
#include <game/entity/location.h>
#include "movement.h"

void game_service_character_movement_new_target
(
        struct GameEntityCharacter* character,
        struct GameEntityLocation* target
)
{
        assert(character);
        assert(target);

        character->target_location.x = target->x;
        character->target_location.y = target->y;
        character->target_location.z = target->z;
}

void game_service_character_movement_validate_and_update
(
        struct GameEntityCharacter* character,
        struct GameEntityLocation* location
)
{
        assert(character);
        assert(location);

        /*double dx = character->target_location.x - location->x;
        double dy = character->target_location.y - location->y;
        double d = sqrt(dx * dx + dy * dy);
        double a = atan2(dy, dx);
        int new_x;
        int new_y;

        if (d <= 1) {
                new_x = character->target_location.x;
                new_y = character->target_location.y;
        } else {
                new_x = location->x;// + 0.6 * cos(a);
                new_y = location->y;// + 0.6 * sin(a);
        }*/

        character->current_location.x = location->x;//new_x;
        character->current_location.y = location->y;//new_y;
        character->current_location.z = location->z;
}
