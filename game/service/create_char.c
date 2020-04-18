#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include <game/dto/char.h>
#include <game/service/persist_char.h>
#include <game/service/create_char.h>

void game_service_create_char(struct GameDtoChar* character)
{
        assert(character);

        /*
         * (franco.montenegro)
         * Take these values from char_templates (when we have templates :P)
         */
        character->current_location.x = -71396;
        character->current_location.y = 258272;
        character->current_location.z = -3135;
        character->max_hp = 100;
        character->max_mp = 100;
        character->current_hp = 100;
        character->current_mp = 100;

        /*
         * (franco.montenegro)
         * Do not use rand...
         */
        character->char_id = rand();

        character->active = 1;
        character->level = 1;

        log_info("Creating new character");
        log_info("ID: %d", character->char_id);
        log_info("Name: %s", character->name);
        log_info("Race: %d", character->race_id);
        log_info("Sex: %d", character->sex);
        log_info("Class id: %d", character->class_id);
        log_info("Int: %d", character->_int);
        log_info("Str: %d", character->str);
        log_info("Con: %d", character->con);
        log_info("Men: %d", character->men);
        log_info("Dex: %d", character->dex);
        log_info("Wit: %d", character->wit);
        log_info("Hair style: %d", character->hair_style);
        log_info("Hair color: %d", character->hair_color);
        log_info("Face: %d", character->face);

        game_service_persist_char(character);
}
