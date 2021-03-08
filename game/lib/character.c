#include "headers.h"
#include "server_packet/say.h"
#include "character.h"

void character_move(character_t *character, vec3_t *target)
{
        assert(character);
        assert(target);

        printf("Moving character from:\n");
        printf("(%d, %d, %d)\n", character->x, character->y, character->z);

        printf("to:\n");
        printf("(%d, %d, %d)\n", target->x, target->y, target->z);

        character->target_x = target->x;
        character->target_y = target->y;
        character->target_z = target->z;
}

void character_move_forward(character_t *character)
{
        int    dx        = 0;
        int    dy        = 0;
        double d         = 0;
        double a         = 0;
        double run_speed = 0;
        double movement  = 0;

        dx = character->target_x - character->x;
        dy = character->target_y - character->y;
        d  = sqrt(dx * dx + dy * dy);

        if (d <= 10) {
                character->x = character->target_x;
                character->y = character->target_y;
                character->z = character->target_z;
                return;
        }

        a         = atan2(dy, dx);
        run_speed = 120;
        movement  = run_speed < d ? run_speed : d;

        character->x += (int) (movement * cos(a));
        character->y += (int) (movement * sin(a));
}

void character_validate_position(
        character_t *character,
        position_t * validated_position,
        position_t * position_to_validate,
        int          heading)
{
        unsigned int character_speed    = 0;
        position_t   character_position = { 0 };
        unsigned int distance           = 0;

        assert(character);
        assert(validated_position);
        assert(position_to_validate);

        if (heading) {
        }

        character_speed      = 120;
        character_position.x = character->x;
        character_position.y = character->y;
        character_position.z = position_to_validate->z;
        distance = position_distance(position_to_validate, &character_position);

        /*
         * If it's close enough we don't
         * apply any correction.
         */
        if (distance < character_speed * 1.5) {
                validated_position->x = character->x;
                validated_position->y = character->y;
                validated_position->z = character->z;
                return;
        }

        validated_position->x = position_to_validate->x;
        validated_position->y = position_to_validate->y;
        validated_position->z = position_to_validate->z;
}

void character_update_state(
        character_t *       character,
        enum CharacterState new_state)
{
        assert(character);
        character->state = new_state;
}
