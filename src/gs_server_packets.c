#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_server_packets.h"

void gs_packet_begin_rotation_pack(packet_t *dest,
                                   struct gs_packet_begin_rotation *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->obj_id);

    type = 0x62;

    packet_append_val(dest, type);
    packet_append_val(dest, src->obj_id);
    packet_append_val(dest, src->degree);
    packet_append_val(dest, src->side);
}

void gs_packet_attack_pack(packet_t *dest, struct gs_packet_attack *src)
{
    byte_t type = 0x00;

    u16_t updated_hit_count = 0;

    assert(dest);
    assert(src);
    assert(src->attacker_id);
    assert(src->hits[0].target_id);

    type = 0x05;

    updated_hit_count = src->hit_count - 1;

    packet_append_val(dest, type);
    packet_append_val(dest, src->attacker_id);
    packet_append_val(dest, src->hits[0].target_id);
    packet_append_val(dest, src->hits[0].damage);
    packet_append_val(dest, src->hits[0].flags);
    packet_append_val(dest, src->attacker_x);
    packet_append_val(dest, src->attacker_y);
    packet_append_val(dest, src->attacker_z);
    packet_append_val(dest, updated_hit_count);

    for (u16_t i = 1; i < src->hit_count; i += 1) {
        assert(src->hits[i].target_id);
        packet_append_val(dest, src->hits[i].target_id);
        packet_append_val(dest, src->hits[i].damage);
        packet_append_val(dest, src->hits[i].flags);
    }
}

void gs_packet_auto_attack_pack(packet_t *dest,
                                struct gs_packet_auto_attack *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->target_id);

    type = 0x2b;
    packet_append_val(dest, type);
    packet_append_val(dest, src->target_id);
}

void gs_packet_create_char_pack(packet_t *dest,
                                struct gs_packet_create_char *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x19;

    packet_append_val(dest, type);
    packet_append_val(dest, src->response);
}

void gs_packet_d0_pack(packet_t *dest, struct gs_packet_d0 *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0xFE;

    src->unknown = 0x1B;

    packet_append_val(dest, type);
    packet_append_val(dest, src->unknown);
    packet_append_val(dest, src->manor_size);
}

void gs_packet_die_pack(packet_t *dest, struct gs_packet_die *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->obj_id);

    type = 0x06;

    packet_append_val(dest, type);
    packet_append_val(dest, src->obj_id);
    packet_append_val(dest, src->to_village);
    packet_append_val(dest, src->to_hideaway);
    packet_append_val(dest, src->to_castle);
    packet_append_val(dest, src->to_siege);
    packet_append_val(dest, src->sweepable);
    packet_append_val(dest, src->to_fixed);
}

void gs_packet_leave_world_pack(packet_t *dest)
{
    byte_t type = 0x00;

    assert(dest);

    type = 0x7e;
    packet_append_val(dest, type);
}

void gs_packet_char_select_pack(packet_t *dest,
                                struct gs_packet_char_select *src)
{
    byte_t type = 0x00;

    size_t name_size  = 0;
    size_t title_size = 0;

    assert(dest);
    assert(src);
    assert(src->id);
    assert(src->playOK1);

    type = 0x15;

    name_size  = l2_string_bytes(src->name, sizeof(src->name));
    title_size = l2_string_bytes(src->title, sizeof(src->title));

    packet_append_val(dest, type);
    packet_append_n(dest, (byte_t *) src->name, name_size);
    packet_append_val(dest, src->id);
    packet_append_n(dest, (byte_t *) src->title, title_size);
    packet_append_val(dest, src->playOK1);
    packet_append_val(dest, src->clan_id);
    packet_append_val(dest, src->empty[0]);
    packet_append_val(dest, src->sex);
    packet_append_val(dest, src->race_id);
    packet_append_val(dest, src->class_id);
    packet_append_val(dest, src->active);
    packet_append_val(dest, src->x);
    packet_append_val(dest, src->y);
    packet_append_val(dest, src->z);
    packet_append_val(dest, src->hp);
    packet_append_val(dest, src->mp);
    packet_append_val(dest, src->sp);
    packet_append_val(dest, src->exp);
    packet_append_val(dest, src->level);
    packet_append_val(dest, src->empty[0]);
    packet_append_val(dest, src->empty[0]);
    packet_append_val(dest, src->_int);
    packet_append_val(dest, src->str);
    packet_append_val(dest, src->con);
    packet_append_val(dest, src->men);
    packet_append_val(dest, src->dex);
    packet_append_val(dest, src->wit);
    packet_append_n(dest, (byte_t *) src->empty, sizeof(src->empty[0]) * 36);
    packet_append_val(dest, src->game_time);
    packet_append_n(dest, (byte_t *) src->empty, sizeof(src->empty[0]) * 15);
}

void gs_packet_validate_pos_pack(packet_t *dest,
                                 struct gs_packet_validate_pos *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->id);

    type = 0x61;

    packet_append_val(dest, type);
    packet_append_val(dest, src->id);
    packet_append_val(dest, src->x);
    packet_append_val(dest, src->y);
    packet_append_val(dest, src->z);
    packet_append_val(dest, src->heading);
}

void gs_packet_move_pack(packet_t *dest, struct gs_packet_move *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->id);

    type = 0x01;

    packet_append_val(dest, type);
    packet_append_val(dest, src->id);
    packet_append_val(dest, src->new_x);
    packet_append_val(dest, src->new_y);
    packet_append_val(dest, src->new_z);
    packet_append_val(dest, src->prev_x);
    packet_append_val(dest, src->prev_y);
    packet_append_val(dest, src->prev_z);
}

void gs_packet_protocol_version_pack(packet_t *dest,
                                     struct gs_packet_protocol_version *src)
{
    byte_t type = 0;

    byte_t content[] = {
        0x01,
        // crypt key
        0x94,
        0x35,
        0x00,
        0x00,
        0xa1,
        0x6c,
        0x54,
        0x87,
    };

    assert(dest);
    assert(src);

    type = 0x00;

    packet_append_val(dest, type);
    packet_append(dest, content);
}

void gs_packet_quest_list_pack(packet_t *dest, struct gs_packet_quest_list *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x80;

    packet_append_val(dest, type);
    packet_append_n(dest, src->empty, sizeof(src->empty));
}

void gs_packet_restart_pack(packet_t *dest, struct gs_packet_restart *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x5f;

    packet_append_val(dest, type);
    packet_append_val(dest, src->response);
}

void gs_packet_revive_pack(packet_t *dest, struct gs_packet_revive *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->obj_id);

    type = 0x07;

    packet_append_val(dest, type);
    packet_append_val(dest, src->obj_id);
}

void gs_packet_target_selected_pack(packet_t *dest,
                                    struct gs_packet_target_selected *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->target_id);

    type = 0xa6;

    packet_append_val(dest, type);
    packet_append_val(dest, src->target_id);
    packet_append_val(dest, src->color);
}

void gs_packet_auth_login_pack(packet_t *dest, struct gs_packet_auth_login *src)
{
    byte_t type = 0;

    struct gs_packet_auth_login_char *character = 0;

    size_t name_size = 0;

    assert(dest);
    assert(src);

    type = 0x13;

    packet_append_val(dest, type);
    packet_append_val(dest, src->count);

    for (u32_t i = 0; i < src->count; i += 1) {
        character = &src->characters[i];
        name_size = l2_string_bytes(character->name, sizeof(character->name));

        packet_append_n(dest, (byte_t *) character->name, name_size);
        packet_append_val(dest, character->id);
        packet_append_n(dest, (byte_t *) character->name, name_size);
        packet_append_val(dest, character->playOK1);
        packet_append_val(dest, character->clan_id);
        packet_append_val(dest, character->empty[0]);
        packet_append_val(dest, character->sex);
        packet_append_val(dest, character->race_id);
        packet_append_val(dest, character->class_id);
        packet_append_val(dest, character->active);
        packet_append_val(dest, character->x);
        packet_append_val(dest, character->y);
        packet_append_val(dest, character->z);
        packet_append_val(dest, character->hp);
        packet_append_val(dest, character->mp);
        packet_append_val(dest, character->sp);
        packet_append_val(dest, character->exp);
        packet_append_val(dest, character->level);
        packet_append_val(dest, character->karma);

        packet_append_n(dest,
                        (byte_t *) character->empty,
                        9 * sizeof(character->empty[0]));

        packet_append_val(dest, character->under_obj_id);
        packet_append_val(dest, character->r_ear_obj_id);
        packet_append_val(dest, character->l_ear_obj_id);
        packet_append_val(dest, character->neck_obj_id);
        packet_append_val(dest, character->r_finger_obj_id);
        packet_append_val(dest, character->l_finger_obj_id);
        packet_append_val(dest, character->head_obj_id);
        packet_append_val(dest, character->r_hand_obj_id);
        packet_append_val(dest, character->l_hand_obj_id);
        packet_append_val(dest, character->gloves_obj_id);
        packet_append_val(dest, character->chest_obj_id);
        packet_append_val(dest, character->legs_obj_id);
        packet_append_val(dest, character->feet_obj_id);
        packet_append_val(dest, character->back_obj_id);
        packet_append_val(dest, character->lr_hand_obj_id);
        packet_append_val(dest, character->hair_obj_id);

        packet_append_val(dest, character->under);
        packet_append_val(dest, character->r_ear);
        packet_append_val(dest, character->l_ear);
        packet_append_val(dest, character->neck);
        packet_append_val(dest, character->r_finger);
        packet_append_val(dest, character->l_finger);
        packet_append_val(dest, character->head);
        packet_append_val(dest, character->r_hand);
        packet_append_val(dest, character->l_hand);
        packet_append_val(dest, character->gloves);
        packet_append_val(dest, character->chest);
        packet_append_val(dest, character->legs);
        packet_append_val(dest, character->feet);
        packet_append_val(dest, character->back);
        packet_append_val(dest, character->lr_hand);
        packet_append_val(dest, character->hair);

        packet_append_val(dest, character->hair_style_id);
        packet_append_val(dest, character->hair_color_id);
        packet_append_val(dest, character->face);
        packet_append_val(dest, character->max_hp);
        packet_append_val(dest, character->max_mp);
        packet_append_val(dest, character->delete_days);
        packet_append_val(dest, character->class_id);
        packet_append_val(dest, character->auto_select);
        packet_append_val(dest, character->enchant_effect);
    }
}

void gs_packet_char_info_pack(packet_t *dest, struct gs_packet_char_info *src)
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

    name_size  = l2_string_bytes(src->name, sizeof(src->name));
    title_size = l2_string_bytes(src->title, sizeof(src->title));

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

void gs_packet_new_char_pack(packet_t *dest, struct gs_packet_new_char *src)
{
    byte_t type = 0x0;

    assert(dest);
    assert(src);

    type = 0x17;

    packet_append_val(dest, type);
    packet_append_val(dest, src->count);

    for (u32_t i = 0; i < src->count; i += 1) {
        packet_append_val(dest, src->templates[i].race);
        packet_append_val(dest, src->templates[i]._class);
        packet_append_val(dest, src->templates[i].separator_46[0]);

        packet_append_val(dest, src->templates[i].str);
        packet_append_val(dest, src->templates[i].separator_0a[1]);
        packet_append_val(dest, src->templates[i].separator_46[1]);

        packet_append_val(dest, src->templates[i].dex);
        packet_append_val(dest, src->templates[i].separator_0a[2]);
        packet_append_val(dest, src->templates[i].separator_46[2]);

        packet_append_val(dest, src->templates[i].con);
        packet_append_val(dest, src->templates[i].separator_0a[3]);
        packet_append_val(dest, src->templates[i].separator_46[3]);

        packet_append_val(dest, src->templates[i]._int);
        packet_append_val(dest, src->templates[i].separator_0a[4]);
        packet_append_val(dest, src->templates[i].separator_46[4]);

        packet_append_val(dest, src->templates[i].wit);
        packet_append_val(dest, src->templates[i].separator_0a[5]);
        packet_append_val(dest, src->templates[i].separator_46[5]);

        packet_append_val(dest, src->templates[i].men);
        packet_append_val(dest, src->templates[i].separator_0a[0]);
    }
}

void gs_packet_npc_info_pack(packet_t *dest, struct gs_packet_npc_info *src)
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

    name_size  = l2_string_bytes(src->name, sizeof(src->name));
    title_size = l2_string_bytes(src->title, sizeof(src->title));

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

void gs_packet_status_pack(packet_t *dest, struct gs_packet_status *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->obj_id);
    assert(src->count);

    type = 0x0e;

    packet_append_val(dest, type);
    packet_append_val(dest, src->obj_id);
    packet_append_val(dest, src->count);

    for (u32_t i = 0; i < src->count; i += 1) {
        packet_append_val(dest, src->attributes[i].type);
        packet_append_val(dest, src->attributes[i].value);
    }
}

void gs_packet_enter_world_pack(packet_t *dest,
                                struct gs_packet_enter_world *src)
{
    byte_t type = 0x00;

    size_t name_size  = 0;
    size_t title_size = 0;

    assert(dest);
    assert(src);

    type = 0x04;

    name_size  = l2_string_bytes(src->name, sizeof(src->name));
    title_size = l2_string_bytes(src->title, sizeof(src->title));

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

    packet_append_val(dest, src->p_attack);
    packet_append_val(dest, src->p_attack_speed);
    packet_append_val(dest, src->p_def);
    packet_append_val(dest, src->evasion_rate);
    packet_append_val(dest, src->accuracy);
    packet_append_val(dest, src->critical_hit);
    packet_append_val(dest, src->m_attack);
    packet_append_val(dest, src->m_attack_speed);
    packet_append_val(dest, src->p_attack_speed);
    packet_append_val(dest, src->m_def);
    packet_append_val(dest, src->pvp_flag); // 0 = normal, 1 = violet
    packet_append_val(dest, src->karma);
    packet_append_val(dest, src->run_speed);
    packet_append_val(dest, src->walk_speed);
    packet_append_val(dest, src->run_speed);  // swim_run_speed
    packet_append_val(dest, src->walk_speed); // swim_walk_speed
    packet_append_val(dest, src->run_speed);  // fly_run_speed
    packet_append_val(dest, src->walk_speed); // fly_walk_speed
    packet_append_val(dest, src->run_speed);  // fly_run_speed
    packet_append_val(dest, src->walk_speed); // fly_walk_speed
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
    packet_append_val(dest, src->party_members); // 1 find party
    packet_append_val(dest, src->abnormal_effect);
    packet_append_n(dest, src->char_empty, 1);
    packet_append_val(dest, src->clan_privileges);
    packet_append_n(dest, (byte_t *) src->int_empty, sizeof(i32_t) * 7);
    packet_append_val(dest, src->recommendation_left);
    packet_append_val(dest, src->recommendation_have);
    packet_append_val(dest, src->int_empty[0]);
    packet_append_val(dest, src->inventory_limit);
    packet_append_val(dest, src->class_id);
    packet_append_val(dest, src->int_empty[0]);
    packet_append_val(dest, src->max_cp);
    packet_append_val(dest, src->cp);
    packet_append_val(dest, src->mounted);
    packet_append_val(dest, src->char_empty[0]); // 1 = blue, 2 = red
    packet_append_val(dest, src->clan_crest_large_id);
    packet_append_val(dest, src->hero_symbol);
    packet_append_val(dest, src->hero);
    packet_append_n(dest, src->char_empty, 1);
    packet_append_val(dest, src->fish_x);
    packet_append_val(dest, src->fish_y);
    packet_append_val(dest, src->fish_z);
    packet_append_val(dest, src->name_color);
}

void gs_packet_say_pack(packet_t *dest, struct gs_packet_say *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->character_id);

    type = 0x4a;

    packet_append_val(dest, type);
    packet_append_val(dest, src->character_id);
    packet_append_val(dest, src->type);
    packet_append_n(dest,
                    src->name,
                    l2_string_bytes(src->name, sizeof(src->name)));
    packet_append_n(dest,
                    src->message,
                    l2_string_bytes(src->message, sizeof(src->message)));
}

void gs_packet_auto_attack_stop_pack(packet_t *dest,
                                     struct gs_packet_auto_attack_stop *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->target_id);

    type = 0x2c;

    packet_append_val(dest, type);
    packet_append_val(dest, src->target_id);
}

void gs_packet_unselect_target_pack(packet_t *dest,
                                    struct gs_packet_unselect_target *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->target_id);

    type = 0x2a;

    packet_append_val(dest, type);
    packet_append_val(dest, src->target_id);
    packet_append_val(dest, src->target_x);
    packet_append_val(dest, src->target_y);
    packet_append_val(dest, src->target_z);
}

void gs_packet_npc_html_message_pack(packet_t *dest,
                                     struct gs_packet_npc_html_message *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x0f;

    packet_append_val(dest, type);
    packet_append_val(dest, src->message_id);
    packet_append_n(dest,
                    src->message,
                    l2_string_bytes(src->message, sizeof(src->message)));
}

void gs_packet_change_move_type_pack(packet_t *dest,
                                     struct gs_packet_change_move_type *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->obj_id);

    type = 0x2e;

    packet_append_val(dest, type);
    packet_append_val(dest, src->obj_id);
    packet_append_val(dest, src->running);
    packet_append_val(dest, src->empty);
}
