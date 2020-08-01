#include "../dto/vec3.h"
#include "character.h"

void character_entity_run_to(struct Character *character, struct Vec3 *location)
{
        character->x = location->x;
        character->y = location->y;
        character->z = location->z;
}

void character_entity_walk_to(struct Character *character, struct Vec3 *location)
{
        character->x = location->x;
        character->y = location->y;
        character->z = location->z;
}
