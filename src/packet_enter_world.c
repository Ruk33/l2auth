#include "include/util.h"
#include "include/l2_string.h"
#include "include/packet.h"
#include "include/character.h"
#include "include/packet_enter_world.h"

void packet_enter_world_set_char(packet_enter_world_t *dest, character_t *src)
{
        dest->x             = 42; // src->x;
        dest->y             = 42; // src->y;
        dest->z             = 42; // src->z;
        dest->id            = 42; // src->id;
        dest->race_id       = src->race;
        dest->sex           = src->sex;
        dest->class_id      = src->_class;
        dest->class_id_copy = src->_class;
        dest->level         = 1; // src->level;
        dest->exp           = 0; // src->exp;
        dest->str           = src->str;
        dest->dex           = src->dex;
        dest->con           = src->con;
        dest->_int          = src->_int;
        dest->wit           = src->wit;
        dest->men           = src->men;
        dest->hp            = (int) 400; // src->hp;
        dest->mp            = (int) 400; // src->mp;
        dest->max_hp        = (int) 400; // src->max_hp;
        dest->max_mp        = (int) 400; // src->max_mp;
        dest->cp            = 400;       // src->cp;
        dest->max_cp        = 400;       // src->max_cp;
        dest->sp            = 0;         // src->sp;
        dest->exp           = 0;         // src->exp;
        dest->sp            = 0;         // src->sp;
        dest->p_attack      = 0;         // src->p_attack;
        dest->m_attack      = 0;         // src->m_attack;
        dest->p_def         = 0;         // src->p_def;
        dest->m_def         = 0;         // src->m_def;
        dest->evasion_rate  = 0;         // src->evasion_rate;
        dest->critical_hit  = 0;         // src->critical_hit;

        dest->run_speed                 = 120;
        dest->walk_speed                = 100;
        dest->p_attack_speed            = 20;
        dest->p_attack_speed_copy       = 20;
        dest->movement_speed_multiplier = 1;
        dest->attack_speed_multiplier   = 1;
        dest->collision_radius          = 9;
        dest->collision_height          = 23;
        dest->name_color                = 0xFFFFFF;
        dest->max_load                  = 42;
}

void packet_enter_world_pack(packet_t *dest, packet_enter_world_t *src)
{
        byte_t type = 0x00;

        size_t name_size  = 0;
        size_t title_size = 0;

        type = 0x04;

        name_size  = l2_string_bytes(src->name);
        title_size = l2_string_bytes(src->title);

        src->unknown = 0x28;

        packet_append_val(dest, type);
        packet_append_val(dest, src->x);
        packet_append_val(dest, src->y);
        packet_append_val(dest, src->z);
        packet_append_val(dest, src->heading);
        packet_append_val(dest, src->id);
        packet_append_n(dest, (byte_t *) src->name, name_size);
        packet_append_val(dest, src->race_id);
        packet_append_val(dest, src->sex);
        packet_append_val(dest, src->class_id);
        packet_append_val(dest, src->level);
        packet_append_val(dest, src->exp);
        packet_append_val(dest, src->str);
        packet_append_val(dest, src->dex);
        packet_append_val(dest, src->con);
        packet_append_val(dest, src->_int);
        packet_append_val(dest, src->wit);
        packet_append_val(dest, src->men);
        packet_append_val(dest, src->max_hp);
        packet_append_val(dest, src->hp);
        packet_append_val(dest, src->max_mp);
        packet_append_val(dest, src->mp);
        packet_append_val(dest, src->sp);
        packet_append_val(dest, src->current_load);
        packet_append_val(dest, src->max_load);
        packet_append_val(dest, src->unknown);
        packet_append_val(dest, src->paperdoll_under);
        packet_append_val(dest, src->paperdoll_r_ear);
        packet_append_val(dest, src->paperdoll_l_ear);
        packet_append_val(dest, src->paperdoll_neck);
        packet_append_val(dest, src->paperdoll_r_finger);
        packet_append_val(dest, src->paperdoll_l_finger);
        packet_append_val(dest, src->paperdoll_head);
        packet_append_val(dest, src->paperdoll_r_hand);
        packet_append_val(dest, src->paperdoll_l_hand);
        packet_append_val(dest, src->paperdoll_gloves);
        packet_append_val(dest, src->paperdoll_chest);
        packet_append_val(dest, src->paperdoll_legs);
        packet_append_val(dest, src->paperdoll_feet);
        packet_append_val(dest, src->paperdoll_back);
        packet_append_val(dest, src->paperdoll_lr_hand);
        packet_append_val(dest, src->paperdoll_hair);
        packet_append_n(dest, (byte_t *) src->empty, sizeof(src->empty));
        packet_append_val(dest, src->p_attack);
        packet_append_val(dest, src->p_attack_speed);
        packet_append_val(dest, src->p_def);
        packet_append_val(dest, src->evasion_rate);
        packet_append_val(dest, src->accuracy);
        packet_append_val(dest, src->critical_hit);
        packet_append_val(dest, src->m_attack);
        packet_append_val(dest, src->m_attack_speed);
        packet_append_val(dest, src->p_attack_speed_copy);
        packet_append_val(dest, src->m_def);
        packet_append_val(dest, src->pvp);
        packet_append_val(dest, src->karma);
        packet_append_val(dest, src->run_speed);
        packet_append_val(dest, src->walk_speed);
        packet_append_val(dest, src->swim_run_speed);
        packet_append_val(dest, src->swim_walk_speed);
        packet_append_val(dest, src->fly_run_speed);
        packet_append_val(dest, src->fly_walk_speed);
        packet_append_val(dest, src->fly_run_speed_copy);
        packet_append_val(dest, src->fly_walk_speed_copy);
        packet_append_val(dest, src->movement_speed_multiplier);
        packet_append_val(dest, src->attack_speed_multiplier);
        packet_append_val(dest, src->collision_radius);
        packet_append_val(dest, src->collision_height);
        packet_append_val(dest, src->hair_style_id);
        packet_append_val(dest, src->hair_color_id);
        packet_append_val(dest, src->face);
        packet_append_val(dest, src->access_level);
        packet_append_n(dest, (byte_t *) src->title, title_size);
        packet_append_val(dest, src->clan_id);
        packet_append_val(dest, src->clan_crest_id);
        packet_append_val(dest, src->ally_id);
        packet_append_val(dest, src->ally_crest_id);
        packet_append_val(dest, src->clan_leader);
        packet_append_val(dest, src->mount_type);
        packet_append_val(dest, src->private_store_type);
        packet_append_val(dest, src->dwarven_craft);
        packet_append_val(dest, src->pk_kills);
        packet_append_val(dest, src->pvp_kills);
        packet_append_val(dest, src->cubics);
        packet_append_val(dest, src->party_members);
        packet_append_val(dest, src->abnormal_effect);
        packet_append_val(dest, src->char_empty[0]);
        packet_append_val(dest, src->clan_privileges);
        packet_append_n(
                dest, (byte_t *) src->int_empty, sizeof(src->int_empty[0]) * 7);
        packet_append_val(dest, src->recommendation_left);
        packet_append_val(dest, src->recommendation_have);
        packet_append_val(dest, src->int_empty[0]);
        packet_append_val(dest, src->inventory_limit);
        packet_append_val(dest, src->class_id_copy);
        packet_append_val(dest, src->int_empty[0]);
        packet_append_val(dest, src->max_cp);
        packet_append_val(dest, src->cp);
        packet_append_val(dest, src->mounted);
        packet_append_val(dest, src->char_empty[0]);
        packet_append_val(dest, src->clan_crest_large_id);
        packet_append_val(dest, src->hero_symbol);
        packet_append_val(dest, src->hero);
        packet_append_val(dest, src->char_empty[0]);
        packet_append_val(dest, src->fish_x);
        packet_append_val(dest, src->fish_y);
        packet_append_val(dest, src->fish_z);
        packet_append_val(dest, src->name_color);
}
