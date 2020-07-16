#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include <game/entity/player.h>
#include "save.h"
#include "create.h"

void game_service_character_create(struct Player* player)
{
        assert(player);

        /*
         * (franco.montenegro)
         * Take these values from char_templates (when we have templates :P)
         */
        player->character.x = -71396;
        player->character.y = 258272;
        player->character.z = -3135;
        player->character.hp = 100;
        player->character.mp = 100;
        player->character.current_hp = 100;
        player->character.current_mp = 100;

        /*
         * (franco.montenegro)
         * Do not use rand...
         */
        player->character.id = rand();

        player->active = 1;
        player->character.level = 1;

        log_info("Creating new character");
        log_info("ID: %d", player->character.id);
        log_info("Name: '%s'", player->character.name);
        log_info("Race: %d", player->race_id);
        log_info("Sex: %d", player->character.sex);
        log_info("Class id: %d", player->class_id);
        log_info("Int: %d", player->character._int);
        log_info("Str: %d", player->character.str);
        log_info("Con: %d", player->character.con);
        log_info("Men: %d", player->character.men);
        log_info("Dex: %d", player->character.dex);
        log_info("Wit: %d", player->character.wit);
        log_info("Hair style: %d", player->hair_style_id);
        log_info("Hair color: %d", player->hair_color_id);
        log_info("Face: %d", player->face);

        game_service_character_save(player);
}
