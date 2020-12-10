#include "headers.h"
#include "character.h"

void character_move(character_t *character, vec3_t *target)
{
        assert(character);
        assert(target);

        printf("Moving character from:\n");
        printf("(%d, %d, %d)\n", character->x, character->y, character->z);

        printf("to:\n");
        printf("(%d, %d, %d)\n", target->x, target->y, target->z);

        character->x = target->x;
        character->y = target->y;
        character->z = target->z;
}

void character_validate_position(character_t *character, vec3_t *pos, int heading)
{
        assert(character);
        assert(pos);

        printf("Validating position:\n");
        printf("(%d, %d, %d, %d)\n", pos->x, pos->y, pos->z, heading);

        character->x = pos->x;
        character->y = pos->y;
        character->z = pos->z;
}
