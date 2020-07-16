#include <assert.h>
#include <math.h>
#include <game/entity/player.h>
#include <game/entity/vec3.h>
#include "movement.h"

void game_service_character_movement_new_target
(
        struct Player* player,
        struct Vec3* target
)
{
        assert(player);
        assert(target);

        // player->character.target_location.x = target->x;
        // player->character.target_location.y = target->y;
        // player->character.target_location.z = target->z;
}

void game_service_character_movement_validate_and_update
(
        struct Player* player,
        struct Vec3* location
)
{
        assert(player);
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

        player->character.x = location->x;//new_x;
        player->character.y = location->y;//new_y;
        player->character.z = location->z;
}
