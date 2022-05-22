#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/l2_string.h"
#include "include/server_packet.h"

void server_packet_protocol_version_encode(struct packet *dest, struct server_packet_protocol_version *src)
{
    byte content[] = {
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
    packet_set_type(dest, 0x00);
    packet_write(dest, content, sizeof(content));
    packet_write_u16(dest, 0);
}

void server_packet_d0_encode(struct packet *dest, struct server_packet_d0 *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x1B);
    packet_write_u16(dest, 0);
    packet_write_u32(dest, src->manor_size);
}

void server_packet_select_character_encode(struct packet *dest, struct server_packet_select_character *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x15);
    packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
    packet_write_u32(dest, src->id);
    packet_write(dest, src->title.buf, l2_string_size(src->title.buf));
    packet_write_i32(dest, src->playOK1);
    packet_write_u32(dest, src->clan_id);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, src->sex);
    packet_write_u32(dest, src->race_id);
    packet_write_u32(dest, src->class_id);
    packet_write_u32(dest, src->active);
    packet_write_i32(dest, src->position.x);
    packet_write_i32(dest, src->position.y);
    packet_write_i32(dest, src->position.z);
    packet_write_d(dest, src->hp);
    packet_write_d(dest, src->mp);
    packet_write_u32(dest, src->sp);
    packet_write_u32(dest, src->exp);
    packet_write_u32(dest, src->level);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_i32(dest, src->attrs._int);
    packet_write_i32(dest, src->attrs.str);
    packet_write_i32(dest, src->attrs.con);
    packet_write_i32(dest, src->attrs.men);
    packet_write_i32(dest, src->attrs.dex);
    packet_write_i32(dest, src->attrs.wit);
    for (int i = 0; i < 37; i += 1) {
        packet_write_u32(dest, 0);
    }
    packet_write_u32(dest, src->game_time);
    for (int i = 0; i < 16; i += 1) {
        packet_write_u32(dest, 0);
    }
}

void server_packet_quest_list_encode(struct packet *dest, struct server_packet_quest_list *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x80);
    packet_write(dest, src->empty, sizeof(src->empty));
}

void server_packet_validate_position_encode(struct packet *dest, struct server_packet_validate_position *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x61);
    packet_write_u32(dest, src->id);
    packet_write_i32(dest, src->position.x);
    packet_write_i32(dest, src->position.y);
    packet_write_i32(dest, src->position.z);
    packet_write_i32(dest, src->position.heading);
}

void server_packet_move_encode(struct packet *dest, struct server_packet_move *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x01);
    packet_write_u32(dest, src->id);
    packet_write_i32(dest, src->new_pos.x);
    packet_write_i32(dest, src->new_pos.y);
    packet_write_i32(dest, src->new_pos.z);
    packet_write_i32(dest, src->prev_pos.x);
    packet_write_i32(dest, src->prev_pos.y);
    packet_write_i32(dest, src->prev_pos.z);
}

void server_packet_restart_encode(struct packet *dest, struct server_packet_restart *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x5f);
    packet_write_i32(dest, src->response);
}

void server_packet_revive_encode(struct packet *dest, struct server_packet_revive *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x07);
    packet_write_i32(dest, src->obj_id);
}

void server_packet_select_target_encode(struct packet *dest, struct server_packet_select_target *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0xa6);
    packet_write_u32(dest, src->target_id);
    packet_write_u32(dest, src->color);
}

void server_packet_auth_login_encode(struct packet *dest, struct server_packet_auth_login *src)
{
    struct server_packet_auth_login_char *character = 0;

    assert(dest);
    assert(src);
    
    packet_set_type(dest, 0x13);
    packet_write_u32(dest, src->count);

    for (u32 i = 0; i < src->count; i += 1) {
        character = &src->characters[i];
        L2_STRING_FROM_CHAR(character->name.buf, "foo");

        packet_write(dest, character->name.buf, l2_string_size(character->name.buf));
        packet_write_u32(dest, character->id);
        packet_write(dest, character->name.buf, l2_string_size(character->name.buf));
        packet_write_i32(dest, character->playOK1);
        packet_write_u32(dest, character->clan_id);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, character->sex);
        packet_write_u32(dest, character->race_id);
        packet_write_u32(dest, character->class_id);
        packet_write_u32(dest, character->active);
        packet_write_u32(dest, character->position.x);
        packet_write_u32(dest, character->position.y);
        packet_write_u32(dest, character->position.z);
        packet_write_d(dest, character->hp);
        packet_write_d(dest, character->mp);
        packet_write_u32(dest, character->sp);
        packet_write_u32(dest, character->exp);
        packet_write_u32(dest, character->level);
        packet_write_u32(dest, character->karma);

        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);

        packet_write_u32(dest, character->under_obj_id);
        packet_write_u32(dest, character->r_ear_obj_id);
        packet_write_u32(dest, character->l_ear_obj_id);
        packet_write_u32(dest, character->neck_obj_id);
        packet_write_u32(dest, character->r_finger_obj_id);
        packet_write_u32(dest, character->l_finger_obj_id);
        packet_write_u32(dest, character->head_obj_id);
        packet_write_u32(dest, character->r_hand_obj_id);
        packet_write_u32(dest, character->l_hand_obj_id);
        packet_write_u32(dest, character->gloves_obj_id);
        packet_write_u32(dest, character->chest_obj_id);
        packet_write_u32(dest, character->legs_obj_id);
        packet_write_u32(dest, character->feet_obj_id);
        packet_write_u32(dest, character->back_obj_id);
        packet_write_u32(dest, character->lr_hand_obj_id);
        packet_write_u32(dest, character->hair_obj_id);

        packet_write_u32(dest, character->under);
        packet_write_u32(dest, character->r_ear);
        packet_write_u32(dest, character->l_ear);
        packet_write_u32(dest, character->neck);
        packet_write_u32(dest, character->r_finger);
        packet_write_u32(dest, character->l_finger);
        packet_write_u32(dest, character->head);
        packet_write_u32(dest, character->r_hand);
        packet_write_u32(dest, character->l_hand);
        packet_write_u32(dest, character->gloves);
        packet_write_u32(dest, character->chest);
        packet_write_u32(dest, character->legs);
        packet_write_u32(dest, character->feet);
        packet_write_u32(dest, character->back);
        packet_write_u32(dest, character->lr_hand);
        packet_write_u32(dest, character->hair);

        packet_write_u32(dest, character->hair_style_id);
        packet_write_u32(dest, character->hair_color_id);
        packet_write_u32(dest, character->face);
        packet_write_d(dest, character->max_hp);
        packet_write_d(dest, character->max_mp);
        packet_write_u32(dest, character->delete_days);
        packet_write_u32(dest, character->class_id);
        packet_write_u32(dest, character->auto_select);
        packet_write_u8(dest, character->enchant_effect);
    }
}

void server_packet_char_info_encode(struct packet *dest, struct server_packet_char_info *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x03);
    packet_write_i32(dest, src->position.x);
    packet_write_i32(dest, src->position.y);
    packet_write_i32(dest, src->position.z);
    packet_write_i32(dest, src->position.heading);
    packet_write_u32(dest, src->id);
    packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
    packet_write_u32(dest, src->race_id);
    packet_write_u32(dest, src->sex);
    packet_write_u32(dest, src->class_id);
    packet_write_u32(dest, 0);
    // Paperdoll?
    for (int i = 0; i < 10; i += 1) {
        packet_write_u32(dest, 0);
    }
    packet_write_u32(dest, src->pvp_flag);
    packet_write_u32(dest, src->karma);
    packet_write_u32(dest, src->m_attack_speed);
    packet_write_u32(dest, src->p_attack_speed);
    packet_write_u32(dest, src->pvp_flag);
    packet_write_u32(dest, src->karma);
    packet_write_u32(dest, src->run_speed);
    packet_write_u32(dest, src->walk_speed);
    packet_write_u32(dest, src->swim_run_speed);
    packet_write_u32(dest, src->swim_walk_speed);
    packet_write_u32(dest, src->fly_run_speed);
    packet_write_u32(dest, src->fly_walk_speed);
    packet_write_u32(dest, src->fly_run_speed);
    packet_write_u32(dest, src->fly_walk_speed);
    packet_write_d(dest, src->movement_multiplier);
    packet_write_d(dest, src->attack_speed_multiplier);
    packet_write_d(dest, src->collision_radius);
    packet_write_d(dest, src->collision_height);
    packet_write_u32(dest, src->hair_style);
    packet_write_u32(dest, src->hair_color);
    packet_write_u32(dest, src->face);
    packet_write(dest, src->title.buf, l2_string_size(src->title.buf));
    packet_write_u32(dest, src->clan_id);
    packet_write_u32(dest, src->clan_crest_id);
    packet_write_u32(dest, src->ally_id);
    packet_write_u32(dest, src->ally_crest_id);
    packet_write_u32(dest, 0);
    packet_write_u8(dest, src->standing);
    packet_write_u8(dest, src->running);
    packet_write_u8(dest, src->in_combat);
    packet_write_u8(dest, src->alike_dead);
    packet_write_u8(dest, src->invisible);
    packet_write_u8(dest, src->mount_type);
    packet_write_u8(dest, src->private_store_type);
    packet_write_u16(dest, src->cubics);
    packet_write_u8(dest, src->find_party_member);
    packet_write_u32(dest, src->abnormal_effect);
    packet_write_u8(dest, 0);
    packet_write_u16(dest, src->recommendations_left);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u8(dest, src->mounted);
    packet_write_u8(dest, 0);
    packet_write_u32(dest, src->clan_crest_large_id);
    packet_write_u8(dest, src->hero_symbol);
    packet_write_u8(dest, src->hero_aura);
    packet_write_u8(dest, src->fishing);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, src->name_color);
}

void server_packet_new_char_encode(struct packet *dest, struct server_packet_new_char *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x17);
    packet_write_u32(dest, src->count);
    for (u32 i = 0; i < src->count; i += 1) {
        packet_write_u32(dest, src->templates[i].race_id);
        packet_write_u32(dest, src->templates[i].class_id);
        packet_write_u32(dest, 0);

        packet_write_u32(dest, src->templates[i].attrs.str);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, src->templates[i].attrs.dex);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, src->templates[i].attrs.con);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, src->templates[i].attrs._int);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, src->templates[i].attrs.wit);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, src->templates[i].attrs.men);
        packet_write_u32(dest, 0);
    }
}

void server_packet_npc_info_encode(struct packet *dest, struct server_packet_npc_info *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x16);
    packet_write_u32(dest, src->id);
    packet_write_u32(dest, src->template_id);
    packet_write_u32(dest, src->attackable);
    packet_write_i32(dest, src->position.x);
    packet_write_i32(dest, src->position.y);
    packet_write_i32(dest, src->position.z);
    packet_write_i32(dest, src->position.heading);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, src->m_attack_speed);
    packet_write_u32(dest, src->p_attack_speed);
    packet_write_u32(dest, src->run_speed);
    packet_write_u32(dest, src->walk_speed);
    packet_write_u32(dest, src->swim_run_speed);
    packet_write_u32(dest, src->swim_walk_speed);
    packet_write_u32(dest, src->fly_run_speed);
    packet_write_u32(dest, src->fly_walk_speed);
    packet_write_u32(dest, src->fly_run_speed);
    packet_write_u32(dest, src->fly_walk_speed);
    packet_write_d(dest, src->magic_multiplier);
    packet_write_d(dest, (double) src->p_attack_speed / 277.478340719);
    packet_write_d(dest, src->collision_radius);
    packet_write_u32(dest, src->r_hand);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, src->l_hand);
    packet_write_u8(dest, src->name_above_char);
    packet_write_u8(dest, src->running);
    packet_write_u8(dest, src->in_combat);
    packet_write_u8(dest, src->alike_dead);
    packet_write_u8(dest, src->summoned);
    packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
    packet_write(dest, src->title.buf, l2_string_size(src->title.buf));
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, src->abnormal_effect);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    packet_write_u8(dest, 0);
    packet_write_u8(dest, 0);
    packet_write_d(dest, 0);
    packet_write_d(dest, 0);
    packet_write_u32(dest, 0);
}

void server_packet_status_encode(struct packet *dest, struct server_packet_status *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x0e);
    packet_write_u32(dest, src->obj_id);
    packet_write_u32(dest, src->count);
    for (u32 i = 0; i < src->count; i += 1) {
        packet_write_u32(dest, src->attributes[i].type);
        packet_write_u32(dest, src->attributes[i].value);
    }
}

void server_packet_enter_world_encode(struct packet *dest, struct server_packet_enter_world *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x04);
    packet_write_i32(dest, src->position.x);
    packet_write_i32(dest, src->position.y);
    packet_write_i32(dest, src->position.z);
    packet_write_i32(dest, src->position.heading);
    packet_write_u32(dest, src->id);
    packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
    packet_write_u32(dest, src->race_id);
    packet_write_u32(dest, src->sex);
    packet_write_u32(dest, src->class_id);
    packet_write_u32(dest, src->level);
    packet_write_u32(dest, src->exp);
    packet_write_i32(dest, src->attrs.str);
    packet_write_i32(dest, src->attrs.dex);
    packet_write_i32(dest, src->attrs.con);
    packet_write_i32(dest, src->attrs._int);
    packet_write_i32(dest, src->attrs.wit);
    packet_write_i32(dest, src->attrs.men);
    packet_write_u32(dest, src->max_hp);
    packet_write_u32(dest, src->hp);
    packet_write_u32(dest, src->max_mp);
    packet_write_u32(dest, src->mp);
    packet_write_u32(dest, src->sp);
    packet_write_u32(dest, src->current_load);
    packet_write_u32(dest, src->max_load);
    packet_write_u32(dest, 0x28);
    packet_write_u32(dest, src->paperdoll_under);
    packet_write_u32(dest, src->paperdoll_r_ear);
    packet_write_u32(dest, src->paperdoll_l_ear);
    packet_write_u32(dest, src->paperdoll_neck);
    packet_write_u32(dest, src->paperdoll_r_finger);
    packet_write_u32(dest, src->paperdoll_l_finger);
    packet_write_u32(dest, src->paperdoll_head);
    packet_write_u32(dest, src->paperdoll_r_hand);
    packet_write_u32(dest, src->paperdoll_l_hand);
    packet_write_u32(dest, src->paperdoll_gloves);
    packet_write_u32(dest, src->paperdoll_chest);
    packet_write_u32(dest, src->paperdoll_legs);
    packet_write_u32(dest, src->paperdoll_feet);
    packet_write_u32(dest, src->paperdoll_back);
    packet_write_u32(dest, src->paperdoll_lr_hand);
    packet_write_u32(dest, src->paperdoll_hair);
    packet_write_u32(dest, src->paperdoll_under);
    packet_write_u32(dest, src->paperdoll_r_ear);
    packet_write_u32(dest, src->paperdoll_l_ear);
    packet_write_u32(dest, src->paperdoll_neck);
    packet_write_u32(dest, src->paperdoll_r_finger);
    packet_write_u32(dest, src->paperdoll_l_finger);
    packet_write_u32(dest, src->paperdoll_head);
    packet_write_u32(dest, src->paperdoll_r_hand);
    packet_write_u32(dest, src->paperdoll_l_hand);
    packet_write_u32(dest, src->paperdoll_gloves);
    packet_write_u32(dest, src->paperdoll_chest);
    packet_write_u32(dest, src->paperdoll_legs);
    packet_write_u32(dest, src->paperdoll_feet);
    packet_write_u32(dest, src->paperdoll_back);
    packet_write_u32(dest, src->paperdoll_lr_hand);
    packet_write_u32(dest, src->paperdoll_hair);
    packet_write_u32(dest, src->p_attack);
    packet_write_u32(dest, src->p_attack_speed);
    packet_write_u32(dest, src->p_def);
    packet_write_u32(dest, src->evasion_rate);
    packet_write_u32(dest, src->accuracy);
    packet_write_u32(dest, src->critical_hit);
    packet_write_u32(dest, src->m_attack);
    packet_write_u32(dest, src->m_attack_speed);
    packet_write_u32(dest, src->p_attack_speed);
    packet_write_u32(dest, src->m_def);
    packet_write_u32(dest, src->pvp_flag);
    packet_write_u32(dest, src->karma);
    packet_write_u32(dest, src->run_speed);
    packet_write_u32(dest, src->walk_speed);
    packet_write_u32(dest, src->swim_run_speed);
    packet_write_u32(dest, src->swim_walk_speed);
    packet_write_u32(dest, src->fly_run_speed);
    packet_write_u32(dest, src->fly_walk_speed);
    packet_write_u32(dest, src->fly_run_speed);
    packet_write_u32(dest, src->fly_walk_speed);
    packet_write_d(dest, src->movement_speed_multiplier);
    packet_write_d(dest, src->attack_speed_multiplier);
    packet_write_d(dest, src->collision_radius);
    packet_write_d(dest, src->collision_height);
    packet_write_u32(dest, src->hair_style_id);
    packet_write_u32(dest, src->hair_color_id);
    packet_write_u32(dest, src->face);
    packet_write_i32(dest, src->access_level);
    packet_write(dest, src->title.buf, l2_string_size(src->title.buf));
    packet_write_u32(dest, src->clan_id);
    packet_write_u32(dest, src->clan_crest_id);
    packet_write_u32(dest, src->ally_id);
    packet_write_u32(dest, src->ally_crest_id);
    packet_write_u32(dest, src->clan_leader);
    packet_write_u8(dest, src->mount_type);
    packet_write_u8(dest, src->private_store_type);
    packet_write_u8(dest, src->dwarven_craft);
    packet_write_u32(dest, src->pk_kills);
    packet_write_u32(dest, src->pvp_kills);
    packet_write_u16(dest, src->cubics);
    packet_write_u8(dest, src->party_members);
    packet_write_u32(dest, src->abnormal_effect);
    packet_write_u8(dest, 0);
    packet_write_u32(dest, src->clan_privileges);
    for (int i = 0; i < 7; i += 1) {
        packet_write_u32(dest, 0);
    }
    packet_write_u16(dest, src->recommendation_left);
    packet_write_u16(dest, src->recommendation_have);
    packet_write_u32(dest, 0);
    packet_write_u16(dest, src->inventory_limit);
    packet_write_u32(dest, src->class_id);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, src->max_cp);
    packet_write_u32(dest, src->cp);
    packet_write_u8(dest, src->mounted);
    packet_write_u8(dest, 0); // 1 = blue, 2 = red
    packet_write_u32(dest, src->clan_crest_large_id);
    packet_write_u8(dest, src->hero_symbol);
    packet_write_u8(dest, src->hero);
    packet_write_u8(dest, 0);
    packet_write_i32(dest, src->fish_x);
    packet_write_i32(dest, src->fish_y);
    packet_write_i32(dest, src->fish_z);
    packet_write_u32(dest, src->name_color);
}

void server_packet_say_encode(struct packet *dest, struct server_packet_say *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x4a);
    packet_write_u32(dest, src->obj_id);
    packet_write_u32(dest, src->type);
    packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
    packet_write(dest, src->message.buf, l2_string_size(src->message.buf));
}

void server_packet_attack_encode(struct packet *dest, struct server_packet_attack *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x05);
    packet_write_u32(dest, src->attacker_id);
    packet_write_u32(dest, src->hits[0].target_id);
    packet_write_u32(dest, src->hits[0].damage);
    packet_write_u8(dest, src->hits[0].flags);
    packet_write_i32(dest, src->attacker_pos.x);
    packet_write_i32(dest, src->attacker_pos.y);
    packet_write_i32(dest, src->attacker_pos.z);
    packet_write_u16(dest, src->hit_count - 1);

    for (u16 i = 1; i < src->hit_count; i += 1) {
        packet_write_u32(dest, src->hits[i].target_id);
        packet_write_u32(dest, src->hits[i].damage);
        packet_write_u8(dest, src->hits[i].flags);
    }
}

void server_packet_stop_auto_attack_encode(struct packet *dest, struct server_packet_stop_auto_attack *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x2c);
    packet_write_u32(dest, src->target_id);
}

void server_packet_deselect_target_encode(struct packet *dest, struct server_packet_deselect_target *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x2a);
    packet_write_u32(dest, src->target_id);
    packet_write_i32(dest, src->target_pos.x);
    packet_write_i32(dest, src->target_pos.y);
    packet_write_i32(dest, src->target_pos.z);
}

void server_packet_die_encode(struct packet *dest, struct server_packet_die *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x06);
    packet_write_u32(dest, src->obj_id);
    packet_write_u32(dest, src->to_village);
    packet_write_u32(dest, src->to_hideaway);
    packet_write_u32(dest, src->to_castle);
    packet_write_u32(dest, src->to_siege);
    packet_write_u32(dest, src->sweepable);
    packet_write_u32(dest, src->to_fixed);
}

void server_packet_npc_html_message_encode(struct packet *dest, struct server_packet_npc_html_message *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x0f);
    packet_write_u32(dest, src->message_id);
    packet_write(dest, src->message.buf, l2_string_size(src->message.buf));
}

void server_packet_change_move_type_encode(struct packet *dest, struct server_packet_change_move_type *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x2e);
    packet_write_u32(dest, src->obj_id);
    packet_write_u32(dest, src->running);
    packet_write_u32(dest, 0);
}

void server_packet_skill_list_encode(struct packet *dest, struct server_packet_skill_list *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x58);
    packet_write_u32(dest, src->count);
    for (u32 i = 0; i < src->count; i += 1) {
        packet_write_u32(dest, src->skills[i].passive);
        packet_write_u32(dest, src->skills[i].level);
        packet_write_u32(dest, src->skills[i].id);
    }
}

void server_packet_use_skill_encode(struct packet *dest, struct server_packet_use_skill *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x48);
    packet_write_u32(dest, src->src_id);
    packet_write_u32(dest, src->target_id);
    packet_write_u32(dest, src->skill_id);
    packet_write_u32(dest, src->skill_level);
    packet_write_u32(dest, src->hit_time);
    packet_write_u32(dest, src->reuse_delay);
    packet_write_i32(dest, src->position.x);
    packet_write_i32(dest, src->position.y);
    packet_write_i32(dest, src->position.z);
    packet_write_u16(dest, 0);
    packet_write_u32(dest, 0);
}
