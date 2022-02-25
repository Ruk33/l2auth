#include <assert.h>
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->obj_id);
    macro_packet_append_val(dest, src->degree);
    macro_packet_append_val(dest, src->side);
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->attacker_id);
    macro_packet_append_val(dest, src->hits[0].target_id);
    macro_packet_append_val(dest, src->hits[0].damage);
    macro_packet_append_val(dest, src->hits[0].flags);
    macro_packet_append_val(dest, src->attacker_x);
    macro_packet_append_val(dest, src->attacker_y);
    macro_packet_append_val(dest, src->attacker_z);
    macro_packet_append_val(dest, updated_hit_count);

    for (u16_t i = 1; i < src->hit_count; i += 1) {
        assert(src->hits[i].target_id);
        macro_packet_append_val(dest, src->hits[i].target_id);
        macro_packet_append_val(dest, src->hits[i].damage);
        macro_packet_append_val(dest, src->hits[i].flags);
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
    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->target_id);
}

void gs_packet_create_char_pack(packet_t *dest,
                                struct gs_packet_create_char *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x19;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->response);
}

void gs_packet_d0_pack(packet_t *dest, struct gs_packet_d0 *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0xFE;

    src->unknown = 0x1B;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->unknown);
    macro_packet_append_val(dest, src->manor_size);
}

void gs_packet_die_pack(packet_t *dest, struct gs_packet_die *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->obj_id);

    type = 0x06;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->obj_id);
    macro_packet_append_val(dest, src->to_village);
    macro_packet_append_val(dest, src->to_hideaway);
    macro_packet_append_val(dest, src->to_castle);
    macro_packet_append_val(dest, src->to_siege);
    macro_packet_append_val(dest, src->sweepable);
    macro_packet_append_val(dest, src->to_fixed);
}

void gs_packet_leave_world_pack(packet_t *dest)
{
    byte_t type = 0x00;

    assert(dest);

    type = 0x7e;
    macro_packet_append_val(dest, type);
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

    macro_packet_append_val(dest, type);
    packet_append_n(dest, (byte_t *) src->name, name_size);
    macro_packet_append_val(dest, src->id);
    packet_append_n(dest, (byte_t *) src->title, title_size);
    macro_packet_append_val(dest, src->playOK1);
    macro_packet_append_val(dest, src->clan_id);
    macro_packet_append_val(dest, src->empty[0]);
    macro_packet_append_val(dest, src->sex);
    macro_packet_append_val(dest, src->race_id);
    macro_packet_append_val(dest, src->class_id);
    macro_packet_append_val(dest, src->active);
    macro_packet_append_val(dest, src->x);
    macro_packet_append_val(dest, src->y);
    macro_packet_append_val(dest, src->z);
    macro_packet_append_val(dest, src->hp);
    macro_packet_append_val(dest, src->mp);
    macro_packet_append_val(dest, src->sp);
    macro_packet_append_val(dest, src->exp);
    macro_packet_append_val(dest, src->level);
    macro_packet_append_val(dest, src->empty[0]);
    macro_packet_append_val(dest, src->empty[0]);
    macro_packet_append_val(dest, src->_int);
    macro_packet_append_val(dest, src->str);
    macro_packet_append_val(dest, src->con);
    macro_packet_append_val(dest, src->men);
    macro_packet_append_val(dest, src->dex);
    macro_packet_append_val(dest, src->wit);
    packet_append_n(dest, (byte_t *) src->empty, sizeof(src->empty[0]) * 36);
    macro_packet_append_val(dest, src->game_time);
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->id);
    macro_packet_append_val(dest, src->x);
    macro_packet_append_val(dest, src->y);
    macro_packet_append_val(dest, src->z);
    macro_packet_append_val(dest, src->heading);
}

void gs_packet_move_pack(packet_t *dest, struct gs_packet_move *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->id);

    type = 0x01;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->id);
    macro_packet_append_val(dest, src->new_x);
    macro_packet_append_val(dest, src->new_y);
    macro_packet_append_val(dest, src->new_z);
    macro_packet_append_val(dest, src->prev_x);
    macro_packet_append_val(dest, src->prev_y);
    macro_packet_append_val(dest, src->prev_z);
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

    macro_packet_append_val(dest, type);
    macro_packet_append(dest, content);
}

void gs_packet_quest_list_pack(packet_t *dest, struct gs_packet_quest_list *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x80;

    macro_packet_append_val(dest, type);
    packet_append_n(dest, src->empty, sizeof(src->empty));
}

void gs_packet_restart_pack(packet_t *dest, struct gs_packet_restart *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x5f;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->response);
}

void gs_packet_revive_pack(packet_t *dest, struct gs_packet_revive *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->obj_id);

    type = 0x07;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->obj_id);
}

void gs_packet_target_selected_pack(packet_t *dest,
                                    struct gs_packet_target_selected *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->target_id);

    type = 0xa6;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->target_id);
    macro_packet_append_val(dest, src->color);
}

void gs_packet_auth_login_pack(packet_t *dest, struct gs_packet_auth_login *src)
{
    byte_t type = 0;

    struct gs_packet_auth_login_char *character = 0;

    size_t name_size = 0;

    assert(dest);
    assert(src);

    type = 0x13;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->count);

    for (u32_t i = 0; i < src->count; i += 1) {
        character = &src->characters[i];
        name_size = l2_string_bytes(character->name, sizeof(character->name));

        packet_append_n(dest, (byte_t *) character->name, name_size);
        macro_packet_append_val(dest, character->id);
        packet_append_n(dest, (byte_t *) character->name, name_size);
        macro_packet_append_val(dest, character->playOK1);
        macro_packet_append_val(dest, character->clan_id);
        macro_packet_append_val(dest, character->empty[0]);
        macro_packet_append_val(dest, character->sex);
        macro_packet_append_val(dest, character->race_id);
        macro_packet_append_val(dest, character->class_id);
        macro_packet_append_val(dest, character->active);
        macro_packet_append_val(dest, character->x);
        macro_packet_append_val(dest, character->y);
        macro_packet_append_val(dest, character->z);
        macro_packet_append_val(dest, character->hp);
        macro_packet_append_val(dest, character->mp);
        macro_packet_append_val(dest, character->sp);
        macro_packet_append_val(dest, character->exp);
        macro_packet_append_val(dest, character->level);
        macro_packet_append_val(dest, character->karma);

        packet_append_n(dest,
                        (byte_t *) character->empty,
                        9 * sizeof(character->empty[0]));

        macro_packet_append_val(dest, character->under_obj_id);
        macro_packet_append_val(dest, character->r_ear_obj_id);
        macro_packet_append_val(dest, character->l_ear_obj_id);
        macro_packet_append_val(dest, character->neck_obj_id);
        macro_packet_append_val(dest, character->r_finger_obj_id);
        macro_packet_append_val(dest, character->l_finger_obj_id);
        macro_packet_append_val(dest, character->head_obj_id);
        macro_packet_append_val(dest, character->r_hand_obj_id);
        macro_packet_append_val(dest, character->l_hand_obj_id);
        macro_packet_append_val(dest, character->gloves_obj_id);
        macro_packet_append_val(dest, character->chest_obj_id);
        macro_packet_append_val(dest, character->legs_obj_id);
        macro_packet_append_val(dest, character->feet_obj_id);
        macro_packet_append_val(dest, character->back_obj_id);
        macro_packet_append_val(dest, character->lr_hand_obj_id);
        macro_packet_append_val(dest, character->hair_obj_id);

        macro_packet_append_val(dest, character->under);
        macro_packet_append_val(dest, character->r_ear);
        macro_packet_append_val(dest, character->l_ear);
        macro_packet_append_val(dest, character->neck);
        macro_packet_append_val(dest, character->r_finger);
        macro_packet_append_val(dest, character->l_finger);
        macro_packet_append_val(dest, character->head);
        macro_packet_append_val(dest, character->r_hand);
        macro_packet_append_val(dest, character->l_hand);
        macro_packet_append_val(dest, character->gloves);
        macro_packet_append_val(dest, character->chest);
        macro_packet_append_val(dest, character->legs);
        macro_packet_append_val(dest, character->feet);
        macro_packet_append_val(dest, character->back);
        macro_packet_append_val(dest, character->lr_hand);
        macro_packet_append_val(dest, character->hair);

        macro_packet_append_val(dest, character->hair_style_id);
        macro_packet_append_val(dest, character->hair_color_id);
        macro_packet_append_val(dest, character->face);
        macro_packet_append_val(dest, character->max_hp);
        macro_packet_append_val(dest, character->max_mp);
        macro_packet_append_val(dest, character->delete_days);
        macro_packet_append_val(dest, character->class_id);
        macro_packet_append_val(dest, character->auto_select);
        macro_packet_append_val(dest, character->enchant_effect);
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->x);
    macro_packet_append_val(dest, src->y);
    macro_packet_append_val(dest, src->z);
    macro_packet_append_val(dest, src->heading);
    macro_packet_append_val(dest, src->id);
    packet_append_n(dest, src->name, name_size);
    macro_packet_append_val(dest, src->race_id);
    macro_packet_append_val(dest, src->sex);
    macro_packet_append_val(dest, src->class_id);
    macro_packet_append_val(dest, empty_int);
    packet_append_n(dest, (byte_t *) paperdoll, sizeof(paperdoll));
    macro_packet_append_val(dest, src->pvp_flag);
    macro_packet_append_val(dest, src->karma);
    macro_packet_append_val(dest, src->m_attack_speed);
    macro_packet_append_val(dest, src->p_attack_speed);
    macro_packet_append_val(dest, src->pvp_flag);
    macro_packet_append_val(dest, src->karma);
    macro_packet_append_val(dest, src->run_speed);
    macro_packet_append_val(dest, src->walk_speed);
    macro_packet_append_val(dest, src->swim_run_speed);
    macro_packet_append_val(dest, src->swim_walk_speed);
    macro_packet_append_val(dest, src->fly_run_speed);
    macro_packet_append_val(dest, src->fly_walk_speed);
    macro_packet_append_val(dest, src->fly_run_speed);
    macro_packet_append_val(dest, src->fly_walk_speed);
    macro_packet_append_val(dest, src->movement_multiplier);
    macro_packet_append_val(dest, src->attack_speed_multiplier);
    macro_packet_append_val(dest, src->collision_radius);
    macro_packet_append_val(dest, src->collision_height);
    macro_packet_append_val(dest, src->hair_style);
    macro_packet_append_val(dest, src->hair_color);
    macro_packet_append_val(dest, src->face);
    packet_append_n(dest, src->title, title_size);
    macro_packet_append_val(dest, src->clan_id);
    macro_packet_append_val(dest, src->clan_crest_id);
    macro_packet_append_val(dest, src->ally_id);
    macro_packet_append_val(dest, src->ally_crest_id);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, src->standing);
    macro_packet_append_val(dest, src->running);
    macro_packet_append_val(dest, src->in_combat);
    macro_packet_append_val(dest, src->alike_dead);
    macro_packet_append_val(dest, src->invisible);
    macro_packet_append_val(dest, src->mount_type);
    macro_packet_append_val(dest, src->private_store_type);
    macro_packet_append_val(dest, src->cubics);
    macro_packet_append_val(dest, src->find_party_member);
    macro_packet_append_val(dest, src->abnormal_effect);
    macro_packet_append_val(dest, empty_byte);
    macro_packet_append_val(dest, src->recommendations_left);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, src->mounted);
    macro_packet_append_val(dest, empty_byte);
    macro_packet_append_val(dest, src->clan_crest_large_id);
    macro_packet_append_val(dest, src->hero_symbol);
    macro_packet_append_val(dest, src->hero_aura);
    macro_packet_append_val(dest, src->fishing);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, src->name_color);
}

void gs_packet_new_char_pack(packet_t *dest, struct gs_packet_new_char *src)
{
    byte_t type = 0x0;

    assert(dest);
    assert(src);

    type = 0x17;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->count);

    for (u32_t i = 0; i < src->count; i += 1) {
        macro_packet_append_val(dest, src->templates[i].race);
        macro_packet_append_val(dest, src->templates[i]._class);
        macro_packet_append_val(dest, src->templates[i].separator_46[0]);

        macro_packet_append_val(dest, src->templates[i].str);
        macro_packet_append_val(dest, src->templates[i].separator_0a[1]);
        macro_packet_append_val(dest, src->templates[i].separator_46[1]);

        macro_packet_append_val(dest, src->templates[i].dex);
        macro_packet_append_val(dest, src->templates[i].separator_0a[2]);
        macro_packet_append_val(dest, src->templates[i].separator_46[2]);

        macro_packet_append_val(dest, src->templates[i].con);
        macro_packet_append_val(dest, src->templates[i].separator_0a[3]);
        macro_packet_append_val(dest, src->templates[i].separator_46[3]);

        macro_packet_append_val(dest, src->templates[i]._int);
        macro_packet_append_val(dest, src->templates[i].separator_0a[4]);
        macro_packet_append_val(dest, src->templates[i].separator_46[4]);

        macro_packet_append_val(dest, src->templates[i].wit);
        macro_packet_append_val(dest, src->templates[i].separator_0a[5]);
        macro_packet_append_val(dest, src->templates[i].separator_46[5]);

        macro_packet_append_val(dest, src->templates[i].men);
        macro_packet_append_val(dest, src->templates[i].separator_0a[0]);
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->id);
    macro_packet_append_val(dest, src->template_id);
    macro_packet_append_val(dest, src->attackable);
    macro_packet_append_val(dest, src->x);
    macro_packet_append_val(dest, src->y);
    macro_packet_append_val(dest, src->z);
    macro_packet_append_val(dest, src->heading);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, src->m_attack_speed);
    macro_packet_append_val(dest, src->p_attack_speed);
    macro_packet_append_val(dest, src->run_speed);
    macro_packet_append_val(dest, src->walk_speed);
    macro_packet_append_val(dest, src->swim_run_speed);
    macro_packet_append_val(dest, src->swim_walk_speed);
    macro_packet_append_val(dest, src->fly_run_speed);
    macro_packet_append_val(dest, src->fly_walk_speed);
    macro_packet_append_val(dest, src->fly_run_speed);
    macro_packet_append_val(dest, src->fly_walk_speed);
    macro_packet_append_val(dest, src->magic_multiplier);
    macro_packet_append_val(dest, p_attack_speed_div);
    macro_packet_append_val(dest, src->collision_radius);
    macro_packet_append_val(dest, src->collision_height);
    macro_packet_append_val(dest, src->r_hand);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, src->l_hand);
    macro_packet_append_val(dest, src->name_above_char);
    macro_packet_append_val(dest, src->running);
    macro_packet_append_val(dest, src->in_combat);
    macro_packet_append_val(dest, src->alike_dead);
    macro_packet_append_val(dest, src->summoned);
    packet_append_n(dest, src->name, name_size);
    packet_append_n(dest, src->title, title_size);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, src->abnormal_effect);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_int);
    macro_packet_append_val(dest, empty_char);
    macro_packet_append_val(dest, empty_char);
    macro_packet_append_val(dest, empty_double);
    macro_packet_append_val(dest, empty_double);
    macro_packet_append_val(dest, empty_int);
}

void gs_packet_status_pack(packet_t *dest, struct gs_packet_status *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->obj_id);
    assert(src->count);

    type = 0x0e;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->obj_id);
    macro_packet_append_val(dest, src->count);

    for (u32_t i = 0; i < src->count; i += 1) {
        macro_packet_append_val(dest, src->attributes[i].type);
        macro_packet_append_val(dest, src->attributes[i].value);
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->x);
    macro_packet_append_val(dest, src->y);
    macro_packet_append_val(dest, src->z);
    macro_packet_append_val(dest, src->heading);
    macro_packet_append_val(dest, src->id);
    packet_append_n(dest, (byte_t *) src->name, name_size);
    macro_packet_append_val(dest, src->race_id);
    macro_packet_append_val(dest, src->sex);
    macro_packet_append_val(dest, src->class_id);
    macro_packet_append_val(dest, src->level);
    macro_packet_append_val(dest, src->exp);
    macro_packet_append_val(dest, src->str);
    macro_packet_append_val(dest, src->dex);
    macro_packet_append_val(dest, src->con);
    macro_packet_append_val(dest, src->_int);
    macro_packet_append_val(dest, src->wit);
    macro_packet_append_val(dest, src->men);
    macro_packet_append_val(dest, src->max_hp);
    macro_packet_append_val(dest, src->hp);
    macro_packet_append_val(dest, src->max_mp);
    macro_packet_append_val(dest, src->mp);
    macro_packet_append_val(dest, src->sp);
    macro_packet_append_val(dest, src->current_load);
    macro_packet_append_val(dest, src->max_load);
    macro_packet_append_val(dest, src->unknown);

    macro_packet_append_val(dest, src->paperdoll_under);
    macro_packet_append_val(dest, src->paperdoll_r_ear);
    macro_packet_append_val(dest, src->paperdoll_l_ear);
    macro_packet_append_val(dest, src->paperdoll_neck);
    macro_packet_append_val(dest, src->paperdoll_r_finger);
    macro_packet_append_val(dest, src->paperdoll_l_finger);

    macro_packet_append_val(dest, src->paperdoll_head);

    macro_packet_append_val(dest, src->paperdoll_r_hand);
    macro_packet_append_val(dest, src->paperdoll_l_hand);
    macro_packet_append_val(dest, src->paperdoll_gloves);
    macro_packet_append_val(dest, src->paperdoll_chest);
    macro_packet_append_val(dest, src->paperdoll_legs);
    macro_packet_append_val(dest, src->paperdoll_feet);
    macro_packet_append_val(dest, src->paperdoll_back);
    macro_packet_append_val(dest, src->paperdoll_lr_hand);
    macro_packet_append_val(dest, src->paperdoll_hair);

    macro_packet_append_val(dest, src->paperdoll_under);
    macro_packet_append_val(dest, src->paperdoll_r_ear);
    macro_packet_append_val(dest, src->paperdoll_l_ear);
    macro_packet_append_val(dest, src->paperdoll_neck);
    macro_packet_append_val(dest, src->paperdoll_r_finger);
    macro_packet_append_val(dest, src->paperdoll_l_finger);

    macro_packet_append_val(dest, src->paperdoll_head);
    macro_packet_append_val(dest, src->paperdoll_r_hand);
    macro_packet_append_val(dest, src->paperdoll_l_hand);
    macro_packet_append_val(dest, src->paperdoll_gloves);
    macro_packet_append_val(dest, src->paperdoll_chest);
    macro_packet_append_val(dest, src->paperdoll_legs);
    macro_packet_append_val(dest, src->paperdoll_feet);
    macro_packet_append_val(dest, src->paperdoll_back);
    macro_packet_append_val(dest, src->paperdoll_lr_hand);
    macro_packet_append_val(dest, src->paperdoll_hair);

    macro_packet_append_val(dest, src->p_attack);
    macro_packet_append_val(dest, src->p_attack_speed);
    macro_packet_append_val(dest, src->p_def);
    macro_packet_append_val(dest, src->evasion_rate);
    macro_packet_append_val(dest, src->accuracy);
    macro_packet_append_val(dest, src->critical_hit);
    macro_packet_append_val(dest, src->m_attack);
    macro_packet_append_val(dest, src->m_attack_speed);
    macro_packet_append_val(dest, src->p_attack_speed);
    macro_packet_append_val(dest, src->m_def);
    macro_packet_append_val(dest, src->pvp_flag); // 0 = normal, 1 = violet
    macro_packet_append_val(dest, src->karma);
    macro_packet_append_val(dest, src->run_speed);
    macro_packet_append_val(dest, src->walk_speed);
    macro_packet_append_val(dest, src->run_speed);  // swim_run_speed
    macro_packet_append_val(dest, src->walk_speed); // swim_walk_speed
    macro_packet_append_val(dest, src->run_speed);  // fly_run_speed
    macro_packet_append_val(dest, src->walk_speed); // fly_walk_speed
    macro_packet_append_val(dest, src->run_speed);  // fly_run_speed
    macro_packet_append_val(dest, src->walk_speed); // fly_walk_speed
    macro_packet_append_val(dest, src->movement_speed_multiplier);
    macro_packet_append_val(dest, src->attack_speed_multiplier);
    macro_packet_append_val(dest, src->collision_radius);
    macro_packet_append_val(dest, src->collision_height);
    macro_packet_append_val(dest, src->hair_style_id);
    macro_packet_append_val(dest, src->hair_color_id);
    macro_packet_append_val(dest, src->face);
    macro_packet_append_val(dest, src->access_level);
    packet_append_n(dest, (byte_t *) src->title, title_size);
    macro_packet_append_val(dest, src->clan_id);
    macro_packet_append_val(dest, src->clan_crest_id);
    macro_packet_append_val(dest, src->ally_id);
    macro_packet_append_val(dest, src->ally_crest_id);
    macro_packet_append_val(dest, src->clan_leader);
    macro_packet_append_val(dest, src->mount_type);
    macro_packet_append_val(dest, src->private_store_type);
    macro_packet_append_val(dest, src->dwarven_craft);
    macro_packet_append_val(dest, src->pk_kills);
    macro_packet_append_val(dest, src->pvp_kills);
    macro_packet_append_val(dest, src->cubics);
    macro_packet_append_val(dest, src->party_members); // 1 find party
    macro_packet_append_val(dest, src->abnormal_effect);
    packet_append_n(dest, src->char_empty, 1);
    macro_packet_append_val(dest, src->clan_privileges);
    packet_append_n(dest, (byte_t *) src->int_empty, sizeof(i32_t) * 7);
    macro_packet_append_val(dest, src->recommendation_left);
    macro_packet_append_val(dest, src->recommendation_have);
    macro_packet_append_val(dest, src->int_empty[0]);
    macro_packet_append_val(dest, src->inventory_limit);
    macro_packet_append_val(dest, src->class_id);
    macro_packet_append_val(dest, src->int_empty[0]);
    macro_packet_append_val(dest, src->max_cp);
    macro_packet_append_val(dest, src->cp);
    macro_packet_append_val(dest, src->mounted);
    macro_packet_append_val(dest, src->char_empty[0]); // 1 = blue, 2 = red
    macro_packet_append_val(dest, src->clan_crest_large_id);
    macro_packet_append_val(dest, src->hero_symbol);
    macro_packet_append_val(dest, src->hero);
    packet_append_n(dest, src->char_empty, 1);
    macro_packet_append_val(dest, src->fish_x);
    macro_packet_append_val(dest, src->fish_y);
    macro_packet_append_val(dest, src->fish_z);
    macro_packet_append_val(dest, src->name_color);
}

void gs_packet_say_pack(packet_t *dest, struct gs_packet_say *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->character_id);

    type = 0x4a;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->character_id);
    macro_packet_append_val(dest, src->type);
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->target_id);
}

void gs_packet_unselect_target_pack(packet_t *dest,
                                    struct gs_packet_unselect_target *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->target_id);

    type = 0x2a;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->target_id);
    macro_packet_append_val(dest, src->target_x);
    macro_packet_append_val(dest, src->target_y);
    macro_packet_append_val(dest, src->target_z);
}

void gs_packet_npc_html_message_pack(packet_t *dest,
                                     struct gs_packet_npc_html_message *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x0f;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->message_id);
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->obj_id);
    macro_packet_append_val(dest, src->running);
    macro_packet_append_val(dest, src->empty);
}

void gs_packet_skill_list_pack(packet_t *dest, struct gs_packet_skill_list *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x58;
    macro_packet_append_val(dest, type);

    macro_packet_append_val(dest, src->count);

    for (u32_t i = 0; i < src->count; i += 1) {
        assert(src->skills[i].id && "skill id is required in skill list.");
        assert(src->skills[i].level &&
               "skill level is required in skill "
               "list.");
        macro_packet_append_val(dest, src->skills[i].passive);
        macro_packet_append_val(dest, src->skills[i].level);
        macro_packet_append_val(dest, src->skills[i].id);
    }
}

void gs_packet_skill_use_pack(packet_t *dest, struct gs_packet_skill_use *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);
    assert(src->src_id);
    assert(src->target_id);
    assert(src->skill_id);
    assert(src->skill_level);

    type = 0x48;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->src_id);
    macro_packet_append_val(dest, src->target_id);
    macro_packet_append_val(dest, src->skill_id);
    macro_packet_append_val(dest, src->skill_level);
    macro_packet_append_val(dest, src->hit_time);
    macro_packet_append_val(dest, src->reuse_delay);
    macro_packet_append_val(dest, src->x);
    macro_packet_append_val(dest, src->y);
    macro_packet_append_val(dest, src->z);
    macro_packet_append_val(dest, src->empty_short);
    macro_packet_append_val(dest, src->empty_int);
}
