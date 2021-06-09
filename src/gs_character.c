#include <assert.h>
#include "include/config.h"
#include "include/util.h"
#include "include/log.h"
#include "include/l2_string.h"
#include "include/gs_session.h"
#include "include/gs_packet_create_char_request.h"
#include "include/gs_character.h"

static gs_character_t characters[MAX_CLIENTS] = { 0 };

static size_t character_count = 0;

void gs_character_from_request(
        gs_character_t *dest,
        gs_packet_create_char_request_t *src)
{
        assert(dest);
        assert(src);

        l2_string_to_char(dest->name, src->name, sizeof(dest->name));

        dest->race       = src->race;
        dest->_class     = src->_class;
        dest->sex        = src->sex;
        dest->face       = src->face;
        dest->hair_color = src->hair_color;
        dest->hair_style = src->hair_style;
        dest->con        = src->con;
        dest->dex        = src->dex;
        dest->men        = src->men;
        dest->str        = src->str;
        dest->wit        = src->wit;
        dest->_int       = src->_int;

        // Talking Island
        dest->x = -83968;
        dest->y = 244634;
        dest->z = -3730;

        dest->id = 42;

        dest->level                     = 1;
        dest->exp                       = 50;
        dest->sp                        = 10;
        dest->hp                        = 400;
        dest->mp                        = 400;
        dest->cp                        = 200;
        dest->max_hp                    = 400;
        dest->max_mp                    = 400;
        dest->max_cp                    = 200;
        dest->p_attack                  = 42;
        dest->m_attack                  = 42;
        dest->p_def                     = 42;
        dest->m_def                     = 42;
        dest->evasion_rate              = 2;
        dest->accuracy                  = 2;
        dest->critical_hit              = 2;
        dest->run_speed                 = 120;
        dest->walk_speed                = 100;
        dest->p_attack_speed            = 20;
        dest->m_attack_speed            = 20;
        dest->movement_speed_multiplier = 1;
        dest->attack_speed_multiplier   = 1;
        dest->collision_radius          = 9;
        dest->collision_height          = 23;
        dest->max_load                  = 42;
        dest->name_color                = 0xFFFFFF;
}

void gs_character_spawn(gs_session_t *session, gs_character_t *src)
{
        assert(session);
        assert(src);

        for (size_t i = 0; i < character_count; i += 1) {
                log("Notify close players.");
        }

        characters[character_count]         = *src;
        characters[character_count].session = session;
        character_count += 1;
}
