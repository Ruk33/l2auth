#include "character.h"
#include "vec3.h"

void character_run_to(struct Character *character, struct Vec3 *location)
{
        character->x = location->x;
        character->y = location->y;
        character->z = location->z;
}

void character_walk_to(struct Character *character, struct Vec3 *location)
{
        character->x = location->x;
        character->y = location->y;
        character->z = location->z;
}
