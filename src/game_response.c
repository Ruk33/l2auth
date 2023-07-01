#include "include/game_response.h"
#include "include/game_request.h"
#include "include/game_server.h"
#include "include/packet.h"

void response_protocol_encode(struct packet *dest, struct response_protocol *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x00);
    packet_write_arr(dest, src->protocol.buf);
    packet_checksum(dest);
}

void response_auth_login_encode(struct packet *dest, struct response_auth_login *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x13);
    packet_write_u32(dest, src->count);
    for (u32 i = 0; i < src->count; i += 1) {
        struct l2_character *character = &src->characters[i];
        
        packet_write(dest, character->name.buf, l2_string_size(character->name.buf, sizeof(character->name)));
        packet_write_u32(dest, character->id);
        packet_write(dest, character->name.buf, l2_string_size(character->name.buf, sizeof(character->name)));
        packet_write_u32(dest, character->play_ok1);
        packet_write_u32(dest, character->clan_id);
        packet_write_u32(dest, 0);
        packet_write_u32(dest, character->sex);
        packet_write_u32(dest, character->race_id);
        packet_write_u32(dest, character->class_id);
        packet_write_u32(dest, character->active);
        packet_write_i32(dest, character->x);
        packet_write_i32(dest, character->y);
        packet_write_i32(dest, character->z);
        packet_write_double(dest, character->hp);
        packet_write_double(dest, character->mp);
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
        
        packet_write_u32(dest, character->under_id);
        packet_write_u32(dest, character->r_ear_id);
        packet_write_u32(dest, character->l_ear_id);
        packet_write_u32(dest, character->neck_id);
        packet_write_u32(dest, character->r_finger_id);
        packet_write_u32(dest, character->l_finger_id);
        packet_write_u32(dest, character->head_id);
        packet_write_u32(dest, character->r_hand_id);
        packet_write_u32(dest, character->l_hand_id);
        packet_write_u32(dest, character->gloves_id);
        packet_write_u32(dest, character->chest_id);
        packet_write_u32(dest, character->legs_id);
        packet_write_u32(dest, character->feet_id);
        packet_write_u32(dest, character->back_id);
        packet_write_u32(dest, character->lr_hand_id);
        packet_write_u32(dest, character->hair_id);
        
        packet_write_u32(dest, character->hair_style_id);
        packet_write_u32(dest, character->hair_color_id);
        packet_write_u32(dest, character->face_id);
        packet_write_double(dest, character->max_hp);
        packet_write_double(dest, character->max_mp);
        packet_write_u32(dest, character->delete_days);
        packet_write_u32(dest, character->class_id);
        packet_write_u32(dest, character->auto_select);
        packet_write_u8(dest, character->enchant_effect);
    }
}

void response_show_creation_screen_encode(struct packet *dest, struct response_show_creation_screen *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x17);
    packet_write_u32(dest, src->count);
}

void response_selected_character_encode(struct packet *dest, struct response_selected_character *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x15);
    packet_write(dest, src->name.buf, l2_string_size(src->name.buf, sizeof(src->name)));
    packet_write_u32(dest, src->id);
    packet_write(dest, src->title.buf, l2_string_size(src->title.buf, sizeof(src->title)));
    packet_write_u32(dest, src->play_ok1);
    packet_write_u32(dest, src->clan_id);
    packet_write_u32(dest, 0);
    packet_write_u32(dest, src->sex);
    packet_write_u32(dest, src->race_id);
    packet_write_u32(dest, src->class_id);
    packet_write_u32(dest, src->active);
    packet_write_i32(dest, src->x);
    packet_write_i32(dest, src->y);
    packet_write_i32(dest, src->z);
    packet_write_double(dest, src->hp);
    packet_write_double(dest, src->mp);
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
    for (int i = 0; i < 37; i++)
        packet_write_u32(dest, 0);
    packet_write_u32(dest, src->game_time);
    for (int i = 0; i < 16; i++)
        packet_write_u32(dest, 0);
}

void response_d0_encode(struct packet *dest, struct response_d0 *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x1B);
    packet_write_u16(dest, 0);
    packet_write_u32(dest, src->manor_size);
}

void response_quest_list_encode(struct packet *dest, struct response_quest_list *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x80);
    packet_write(dest, src->empty, sizeof(src->empty));
}

void response_enter_world_encode(struct packet *dest, struct response_enter_world *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x04);
    packet_write_i32(dest, src->x);
    packet_write_i32(dest, src->y);
    packet_write_i32(dest, src->z);
    packet_write_i32(dest, src->heading);
    packet_write_u32(dest, src->id);
    packet_write(dest, src->name.buf, l2_string_size(src->name.buf, sizeof(src->name.buf)));
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
    packet_write_u32(dest, src->paperdoll_under_id);
    packet_write_u32(dest, src->paperdoll_r_ear_id);
    packet_write_u32(dest, src->paperdoll_l_ear_id);
    packet_write_u32(dest, src->paperdoll_neck_id);
    packet_write_u32(dest, src->paperdoll_r_finger_id);
    packet_write_u32(dest, src->paperdoll_l_finger_id);
    packet_write_u32(dest, src->paperdoll_head_id);
    packet_write_u32(dest, src->paperdoll_r_hand_id);
    packet_write_u32(dest, src->paperdoll_l_hand_id);
    packet_write_u32(dest, src->paperdoll_gloves_id);
    packet_write_u32(dest, src->paperdoll_chest_id);
    packet_write_u32(dest, src->paperdoll_legs_id);
    packet_write_u32(dest, src->paperdoll_feet_id);
    packet_write_u32(dest, src->paperdoll_back_id);
    packet_write_u32(dest, src->paperdoll_lr_hand_id);
    packet_write_u32(dest, src->paperdoll_hair_id);
    packet_write_u32(dest, src->paperdoll_under_id);
    packet_write_u32(dest, src->paperdoll_r_ear_id);
    packet_write_u32(dest, src->paperdoll_l_ear_id);
    packet_write_u32(dest, src->paperdoll_neck_id);
    packet_write_u32(dest, src->paperdoll_r_finger_id);
    packet_write_u32(dest, src->paperdoll_l_finger_id);
    packet_write_u32(dest, src->paperdoll_head_id);
    packet_write_u32(dest, src->paperdoll_r_hand_id);
    packet_write_u32(dest, src->paperdoll_l_hand_id);
    packet_write_u32(dest, src->paperdoll_gloves_id);
    packet_write_u32(dest, src->paperdoll_chest_id);
    packet_write_u32(dest, src->paperdoll_legs_id);
    packet_write_u32(dest, src->paperdoll_feet_id);
    packet_write_u32(dest, src->paperdoll_back_id);
    packet_write_u32(dest, src->paperdoll_lr_hand_id);
    packet_write_u32(dest, src->paperdoll_hair_id);
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
    packet_write_double(dest, src->movement_speed_multiplier);
    packet_write_double(dest, src->attack_speed_multiplier);
    packet_write_double(dest, src->collision_radius);
    packet_write_double(dest, src->collision_height);
    packet_write_u32(dest, src->hair_style_id);
    packet_write_u32(dest, src->hair_color_id);
    packet_write_u32(dest, src->face_id);
    packet_write_i32(dest, src->access_level);
    packet_write(dest, src->title.buf, l2_string_size(src->title.buf, sizeof(src->title.buf)));
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
    for (int i = 0; i < 7; i++)
        packet_write_u32(dest, 0);
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

void response_restart_encode(struct packet *dest, struct response_restart *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x5f);
    packet_write_u32(dest, src->code);
    // todo: l2j sends a string for some reason, we may wanna
    // check what's that all about? (since it seems a random
    // value)
}

void response_move_encode(struct packet *dest, struct response_move *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x01);
    packet_write_u32(dest, src->id);
    packet_write_i32(dest, src->destination_x);
    packet_write_i32(dest, src->destination_y);
    packet_write_i32(dest, src->destination_z);
    packet_write_i32(dest, src->origin_x);
    packet_write_i32(dest, src->origin_y);
    packet_write_i32(dest, src->origin_z);
}

void response_validate_position_encode(struct packet *dest, struct response_validate_position *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x61);
    packet_write_u32(dest, src->id);
    packet_write_i32(dest, src->x);
    packet_write_i32(dest, src->y);
    packet_write_i32(dest, src->z);
    packet_write_i32(dest, src->heading);
}

void response_char_info_encode(struct packet *dest, struct l2_character *character)
{
    assert(dest);
    assert(character);
    // todo: if the character is morphed, we have to send just
    // a few properties, not the entire thing.
    // we are assuming the character is not morphed.
    // WHEN MORPHED, the id of the packet is 0x16.
    packet_set_type(dest, 0x03);
    packet_write_i32(dest, character->x);
    packet_write_i32(dest, character->y);
    packet_write_i32(dest, character->z);
    packet_write_i32(dest, character->heading);
    assert(character->id);
    packet_write_u32(dest, character->id);
    packet_write(dest, character->name.buf, l2_string_size(character->name.buf, sizeof(character->name.buf)));
    packet_write_u32(dest, character->race_id);
    packet_write_u32(dest, character->sex);
    packet_write_u32(dest, character->class_id);
    // unknown.
    packet_write_u32(dest, 0);
    packet_write_u32(dest, character->head_id);
    packet_write_u32(dest, character->r_hand_id);
    packet_write_u32(dest, character->l_hand_id);
    packet_write_u32(dest, character->gloves_id);
    packet_write_u32(dest, character->chest_id);
    packet_write_u32(dest, character->legs_id);
    packet_write_u32(dest, character->feet_id);
    packet_write_u32(dest, character->back_id);
    packet_write_u32(dest, character->lr_hand_id);
    packet_write_u32(dest, character->hair_id);
    // todo: pvp flag.
    packet_write_u32(dest, 0);
    packet_write_u32(dest, character->karma);
    packet_write_u32(dest, character->m_attack_speed);
    packet_write_u32(dest, character->p_attack_speed);
    // todo: pvp flag.
    packet_write_u32(dest, 0);
    packet_write_u32(dest, character->karma);
    
    packet_write_i32(dest, character->run_speed);
    packet_write_i32(dest, character->walk_speed);
    // swim
    packet_write_i32(dest, character->run_speed/2);
    packet_write_i32(dest, character->walk_speed/2);
    // fly
    packet_write_i32(dest, character->run_speed*2);
    packet_write_i32(dest, character->walk_speed*2);
    packet_write_i32(dest, character->run_speed*2);
    packet_write_i32(dest, character->walk_speed*2);
    packet_write_double(dest, character->movement_speed_multiplier);
    packet_write_double(dest, character->attack_speed_multiplier);
    packet_write_double(dest, character->collision_radius);
    packet_write_double(dest, character->collision_height);
    packet_write_u32(dest, character->hair_style_id);
    packet_write_u32(dest, character->hair_color_id);
    packet_write_u32(dest, character->face_id);
    packet_write(dest, character->title.buf, l2_string_size(character->title.buf, sizeof(character->title.buf)));
    packet_write_u32(dest, character->clan_id);
    // todo: clan crest id.
    packet_write_u32(dest, 0);
    // todo: ally id.
    packet_write_u32(dest, 0);
    // todo: ally crest id.
    packet_write_u32(dest, 0);
    // siege flags?
    packet_write_u32(dest, 0);
    // sitting
    packet_write_u8(dest, 1); // standing = 1  sitting = 0
    // running
    packet_write_u8(dest, 1); // running = 1   walking = 0
    // in combat
    packet_write_u8(dest, 0);
    // alike dead
    packet_write_u8(dest, 0);
    // invisible
    packet_write_u8(dest, 0);
    // mount type
    packet_write_u8(dest, 0);
    // private store type
    packet_write_u8(dest, 0);
    // todo: cubics
    packet_write_u16(dest, 0);
    // find party members
    packet_write_u8(dest, 0);
    // abnormal
    packet_write_u32(dest, 0);
    
    packet_write_u8(dest, 0);
    // recommendations have
    packet_write_u16(dest, 0);
    packet_write_u32(dest, 0);
    // unknown
    packet_write_u32(dest, 0);
    packet_write_u32(dest, 0);
    // is mounted
    packet_write_u8(dest, 0);
    // unknown
    packet_write_u8(dest, 0);
    // clan crest large id
    packet_write_u32(dest, 0);
    // hero symbol
    packet_write_u8(dest, 0);
    // hero aura
    packet_write_u8(dest, 0);
    // fishing?
    packet_write_u8(dest, 0);
    // fishing position
    packet_write_i32(dest, character->x);
    packet_write_i32(dest, character->y);
    packet_write_i32(dest, character->z);
    packet_write_u32(dest, character->name_color);
}

void response_action_failed_encode(struct packet *dest)
{
    assert(dest);
    packet_set_type(dest, 0x25);
    // We don't fully support packets with empty bodies so
    // we have to make some room.
    packet_write_u32(dest, 0);
}

void response_action_select_target_encode(struct packet *dest, struct l2_character *target)
{
    assert(dest);
    assert(target);
    packet_set_type(dest, 0xa6);
    packet_write_u32(dest, target->id);
    u16 color = 2; // white;
    packet_write_u16(dest, color);
}

void response_attack_target_encode(struct packet *dest, struct l2_character *attacker, struct l2_character *target)
{
    assert(dest);
    assert(attacker);
    assert(target);
    packet_set_type(dest, 0x05);
    packet_write_u32(dest, attacker->id);
    packet_write_u32(dest, target->id);
    // TODO: calculate damage.
    u32 damage = 10;
    packet_write_u32(dest, damage);
    // 
    // from l2j:
    // if (_soulshot) _flags |= 0x10 | _grade;
    // if (crit)      _flags |= 0x20;
    // if (shld)      _flags |= 0x40;
    // if (miss)      _flags |= 0x80;
    // 
    u8 flags = 0;
    packet_write_u8(dest, flags);
    packet_write_i32(dest, attacker->x);
    packet_write_i32(dest, attacker->y);
    packet_write_i32(dest, attacker->z);
    // todo: properly handle this.
    u16 hits_count = 0;
    packet_write_u16(dest, hits_count);
}

void response_say_encode(struct packet *dest, struct l2_character *from, l2_string *message, size_t n)
{
    assert(dest);
    assert(from);
    assert(message);
    packet_set_type(dest, 0x4a);
    
    packet_write_u32(dest, from->id);
    
    u32 message_type = 0;
    packet_write_u32(dest, message_type);
    
    packet_write(dest, from->name.buf, l2_string_size(from->name.buf, sizeof(from->name.buf)));
    
    packet_write(dest, message, l2_string_size(message, n));
}
