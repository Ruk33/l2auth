#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/l2_string.h"
#include "include/gs_types.h"
#include "include/gs_packet_npc_info.h"

void gs_packet_npc_info(gs_packet_npc_info_t *dest, struct gs_character *src)
{
        assert(dest);
        assert(src);

        l2_string_from_char(dest->name, src->name, sizeof(dest->name));
        l2_string_from_char(dest->title, src->title, sizeof(dest->title));

        dest->id                  = src->id;
        dest->template_id         = 6 + 1000000; // orc
        dest->attackable          = 1;
        dest->x                   = src->position.x;
        dest->y                   = src->position.y;
        dest->z                   = src->position.z;
        dest->heading             = src->heading;
        dest->m_attack_speed      = src->stats.m_attack_speed;
        dest->p_attack_speed      = src->stats.p_attack_speed;
        dest->run_speed           = src->stats.run_speed;
        dest->walk_speed          = src->stats.walk_speed;
        dest->swim_run_speed      = src->stats.run_speed;
        dest->swim_walk_speed     = src->stats.walk_speed;
        dest->fly_run_speed       = src->stats.run_speed;
        dest->fly_walk_speed      = src->stats.walk_speed;
        dest->magic_multiplier    = 1;
        dest->movement_multiplier = src->stats.movement_speed_multiplier;
        dest->collision_radius    = src->collision_radius;
        dest->collision_height    = src->collision_height;
        dest->r_hand              = 0;
        dest->l_hand              = 0;
        dest->name_above_char     = 1;
        dest->running             = 1;
        dest->in_combat           = 0;
        dest->alike_dead          = 0;
        dest->summoned            = 0;
        dest->karma               = 0;
        dest->abnormal_effect     = 0;
        dest->team_circle         = 0;
}

void gs_packet_npc_info_pack(packet_t *dest, gs_packet_npc_info_t *src)
{
        byte_t type = 0x00;

        u32_t empty_int     = 0;
        double empty_double = 0;
        u8_t empty_char     = 0;

        double p_attack_speed_div = 0;

        size_t name_size  = 0;
        size_t title_size = 0;

        assert(dest);
        assert(src);

        type = 0x16;

        p_attack_speed_div = src->p_attack_speed / 277.478340719;

        name_size  = l2_string_bytes(src->name);
        title_size = l2_string_bytes(src->title);

        packet_append_val(dest, type);
        packet_append_val(dest, src->id);
        packet_append_val(dest, src->template_id);
        packet_append_val(dest, src->attackable);
        packet_append_val(dest, src->x);
        packet_append_val(dest, src->y);
        packet_append_val(dest, src->z);
        packet_append_val(dest, src->heading);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, src->m_attack_speed);
        packet_append_val(dest, src->p_attack_speed);
        packet_append_val(dest, src->run_speed);
        packet_append_val(dest, src->walk_speed);
        packet_append_val(dest, src->swim_run_speed);
        packet_append_val(dest, src->swim_walk_speed);
        packet_append_val(dest, src->fly_run_speed);
        packet_append_val(dest, src->fly_walk_speed);
        packet_append_val(dest, src->fly_run_speed);
        packet_append_val(dest, src->fly_walk_speed);
        packet_append_val(dest, src->magic_multiplier);
        packet_append_val(dest, p_attack_speed_div);
        packet_append_val(dest, src->collision_radius);
        packet_append_val(dest, src->collision_height);
        packet_append_val(dest, src->r_hand);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, src->l_hand);
        packet_append_val(dest, src->name_above_char);
        packet_append_val(dest, src->running);
        packet_append_val(dest, src->in_combat);
        packet_append_val(dest, src->alike_dead);
        packet_append_val(dest, src->summoned);
        packet_append_n(dest, src->name, name_size);
        packet_append_n(dest, src->title, title_size);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, src->abnormal_effect);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_char);
        packet_append_val(dest, empty_char);
        packet_append_val(dest, empty_double);
        packet_append_val(dest, empty_double);
        packet_append_val(dest, empty_int);
}
