#include <headers.h>
#include <character.h>
#include "npc_info.h"

#define npc_info_packet_bytes sizeof(server_packet_npc_info_t)

void server_packet_npc_info(packet *dest, character_t *npc)
{
        packet_type type                       = 0x16;
        byte_t      buf[npc_info_packet_bytes] = { 0 };
        byte_t *    p                          = buf;

        char   empty_char   = 0;
        double empty_double = 0;
        int    empty_int    = 0;

        int    orc_archer_template = 0;
        int    template_id         = 0;
        int    attackable          = 0;
        int    heading             = 0;
        int    m_atk_speed         = 0;
        int    p_atk_speed         = 0;
        int    run_speed           = 0;
        int    walk_speed          = 0;
        int    swim_run_speed      = 0;
        int    swim_walk_speed     = 0;
        int    fly_run_speed       = 0;
        int    fly_walk_speed      = 0;
        double magic_multiplier    = 0;
        double p_atk_speed_divided = 0;
        double collision_radius    = 0;
        double collision_height    = 0;
        int    r_hand              = 0;
        int    l_hand              = 0;
        char   name_above_char     = 0;
        char   running             = 0;
        char   in_combat           = 0;
        char   alike_dead          = 0;
        char   summoned            = 0;
        int    abnormal_effect     = 0;

        l2_string_t l2_name[24]  = { 0 };
        l2_string_t l2_title[24] = { 0 };

        assert(dest);
        assert(npc);

        orc_archer_template = 6; // Yay, let's begin with orcs, ORCS!
        template_id         = orc_archer_template + 1000000;
        attackable          = 1;
        m_atk_speed         = 1;
        p_atk_speed         = 1;
        run_speed           = 200;
        walk_speed          = 100;
        magic_multiplier    = 1.1; // Not sure what this is
        p_atk_speed_divided = p_atk_speed / 277.478340719;
        collision_radius    = 14;
        collision_height    = 25;
        name_above_char     = 1;

        l2_string_from_char(l2_name, npc->name, strlen(npc->name) + 1);
        l2_string_from_char(l2_title, npc->title, strlen(npc->title) + 1);

        BYTE_WRITE_VAL(p, npc->id);
        BYTE_WRITE_VAL(p, template_id);
        BYTE_WRITE_VAL(p, attackable);
        BYTE_WRITE_VAL(p, npc->x);
        BYTE_WRITE_VAL(p, npc->y);
        BYTE_WRITE_VAL(p, npc->z);
        BYTE_WRITE_VAL(p, heading);
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, m_atk_speed);
        BYTE_WRITE_VAL(p, p_atk_speed);
        BYTE_WRITE_VAL(p, run_speed);
        BYTE_WRITE_VAL(p, walk_speed);
        BYTE_WRITE_VAL(p, swim_run_speed);
        BYTE_WRITE_VAL(p, swim_walk_speed);
        BYTE_WRITE_VAL(p, fly_run_speed);
        BYTE_WRITE_VAL(p, fly_walk_speed);
        BYTE_WRITE_VAL(p, fly_run_speed);
        BYTE_WRITE_VAL(p, fly_walk_speed);
        BYTE_WRITE_VAL(p, magic_multiplier);
        BYTE_WRITE_VAL(p, p_atk_speed_divided);
        BYTE_WRITE_VAL(p, collision_radius);
        BYTE_WRITE_VAL(p, collision_height);
        BYTE_WRITE_VAL(p, r_hand);
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, l_hand);
        BYTE_WRITE_VAL(p, name_above_char);
        BYTE_WRITE_VAL(p, running);
        BYTE_WRITE_VAL(p, in_combat);
        BYTE_WRITE_VAL(p, alike_dead);
        BYTE_WRITE_VAL(p, summoned);
        BYTE_WRITE(p, l2_name, l2_string_bytes(l2_name));
        BYTE_WRITE(p, l2_title, l2_string_bytes(l2_title));
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, abnormal_effect);
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, empty_int);
        BYTE_WRITE_VAL(p, empty_char);
        BYTE_WRITE_VAL(p, empty_char);
        BYTE_WRITE_VAL(p, empty_double);
        BYTE_WRITE_VAL(p, empty_double);
        BYTE_WRITE_VAL(p, empty_int);

        packet_build(dest, type, buf, (size_t)(p - buf));
}
