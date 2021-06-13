#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/l2_string.h"
#include "include/gs_character.h"
#include "include/gs_packet_char_info.h"

void gs_packet_char_info(gs_packet_char_info_t *dest, gs_character_t *src)
{
        assert(dest);
        assert(src);

        bytes_zero(dest->name, sizeof(dest->name));
        bytes_zero(dest->title, sizeof(dest->title));

        l2_string_from_char(dest->name, src->name, sizeof(dest->name));
        l2_string_from_char(dest->title, src->title, sizeof(dest->title));

        dest->x                       = src->x;
        dest->y                       = src->y;
        dest->z                       = src->z;
        dest->heading                 = src->heading;
        dest->id                      = src->session->id;
        dest->race_id                 = src->race;
        dest->sex                     = src->sex;
        dest->class_id                = src->_class;
        dest->pvp_flag                = 0;
        dest->karma                   = 0;
        dest->m_attack_speed          = src->m_attack_speed;
        dest->p_attack_speed          = src->p_attack_speed;
        dest->run_speed               = src->run_speed;
        dest->walk_speed              = src->walk_speed;
        dest->swim_run_speed          = src->run_speed;
        dest->swim_walk_speed         = src->walk_speed;
        dest->fly_run_speed           = src->run_speed;
        dest->fly_walk_speed          = src->walk_speed;
        dest->movement_multiplier     = src->movement_speed_multiplier;
        dest->attack_speed_multiplier = src->attack_speed_multiplier;
        dest->collision_radius        = src->collision_radius;
        dest->collision_height        = src->collision_height;
        dest->hair_style              = src->hair_style;
        dest->hair_color              = src->hair_color;
        dest->face                    = src->face;
        dest->clan_id                 = 0;
        dest->clan_crest_id           = 0;
        dest->ally_id                 = 0;
        dest->ally_crest_id           = 0;
        dest->standing                = 1;
        dest->running                 = 1;
        dest->in_combat               = 0;
        dest->alike_dead              = 0;
        dest->invisible               = 0;
        dest->mount_type              = 0;
        dest->private_store_type      = 0;
        dest->cubics                  = 0;
        dest->find_party_member       = 0;
        dest->abnormal_effect         = 0;
        dest->recommendations_left    = 0;
        dest->mounted                 = 0;
        dest->clan_crest_large_id     = 0;
        dest->hero_symbol             = 0;
        dest->hero_aura               = 0;
        dest->fishing                 = 0;
        dest->name_color              = 0xFFFFFF;
}

void gs_packet_char_info_pack(packet_t *dest, gs_packet_char_info_t *src)
{
        byte_t type = 0x00;

        size_t name_size  = 0;
        size_t title_size = 0;

        u32_t empty_int     = 0;
        u32_t paperdoll[10] = { 0 };

        byte_t empty_byte = 0;

        assert(dest);
        assert(src);

        type = 0x03;

        name_size  = l2_string_bytes(src->name);
        title_size = l2_string_bytes(src->title);

        packet_append_val(dest, type);
        packet_append_val(dest, src->x);
        packet_append_val(dest, src->y);
        packet_append_val(dest, src->z);
        packet_append_val(dest, src->heading);
        packet_append_val(dest, src->id);
        packet_append_n(dest, src->name, name_size);
        packet_append_val(dest, src->race_id);
        packet_append_val(dest, src->sex);
        packet_append_val(dest, src->class_id);
        packet_append_val(dest, empty_int);
        packet_append_n(dest, (byte_t *) paperdoll, sizeof(paperdoll));
        packet_append_val(dest, src->pvp_flag);
        packet_append_val(dest, src->karma);
        packet_append_val(dest, src->m_attack_speed);
        packet_append_val(dest, src->p_attack_speed);
        packet_append_val(dest, src->pvp_flag);
        packet_append_val(dest, src->karma);
        packet_append_val(dest, src->run_speed);
        packet_append_val(dest, src->walk_speed);
        packet_append_val(dest, src->swim_run_speed);
        packet_append_val(dest, src->swim_walk_speed);
        packet_append_val(dest, src->fly_run_speed);
        packet_append_val(dest, src->fly_walk_speed);
        packet_append_val(dest, src->fly_run_speed);
        packet_append_val(dest, src->fly_walk_speed);
        packet_append_val(dest, src->movement_multiplier);
        packet_append_val(dest, src->attack_speed_multiplier);
        packet_append_val(dest, src->collision_radius);
        packet_append_val(dest, src->collision_height);
        packet_append_val(dest, src->hair_style);
        packet_append_val(dest, src->hair_color);
        packet_append_val(dest, src->face);
        packet_append_n(dest, src->title, title_size);
        packet_append_val(dest, src->clan_id);
        packet_append_val(dest, src->clan_crest_id);
        packet_append_val(dest, src->ally_id);
        packet_append_val(dest, src->ally_crest_id);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, src->standing);
        packet_append_val(dest, src->running);
        packet_append_val(dest, src->in_combat);
        packet_append_val(dest, src->alike_dead);
        packet_append_val(dest, src->invisible);
        packet_append_val(dest, src->mount_type);
        packet_append_val(dest, src->private_store_type);
        packet_append_val(dest, src->cubics);
        packet_append_val(dest, src->find_party_member);
        packet_append_val(dest, src->abnormal_effect);
        packet_append_val(dest, empty_byte);
        packet_append_val(dest, src->recommendations_left);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, src->mounted);
        packet_append_val(dest, empty_byte);
        packet_append_val(dest, src->clan_crest_large_id);
        packet_append_val(dest, src->hero_symbol);
        packet_append_val(dest, src->hero_aura);
        packet_append_val(dest, src->fishing);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, empty_int);
        packet_append_val(dest, src->name_color);
}
