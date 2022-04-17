#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/l2_string.h"
#include "include/server_packet.h"

void packet_protocol_version_to(struct packet *dest, struct packet_protocol_version *src)
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

void packet_d0_to(struct packet *dest, struct packet_d0 *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x1B);
    packet_write_u16(dest, 0);
    packet_write_u32(dest, src->manor_size);
}

void packet_char_select_to(struct packet *dest, struct packet_char_select *src);

void packet_validate_pos_to(struct packet *dest, struct packet_validate_pos *src)
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

void packet_move_to(struct packet *dest, struct packet_move *src)
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

void packet_restart_to(struct packet *dest, struct packet_restart *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x5f);
    packet_write_i32(dest, src->response);
}

void packet_revive_to(struct packet *dest, struct packet_revive *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x07);
    packet_write_i32(dest, src->obj_id);
}

void packet_target_selected_to(struct packet *dest, struct packet_target_selected *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0xa6);
    packet_write_u32(dest, src->target_id);
    packet_write_u32(dest, src->color);
}

void packet_auth_login_char_to(struct packet *dest, struct packet_auth_login_char *src);

void packet_auth_login_to(struct packet *dest, struct packet_auth_login *src)
{
    struct packet_auth_login_char *character = 0;

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

void packet_char_info_to(struct packet *dest, struct packet_char_info *src);

void packet_new_char_template_to(struct packet *dest, struct packet_new_char_template *src);

void packet_new_char_to(struct packet *dest, struct packet_new_char *src);

void packet_npc_info_to(struct packet *dest, struct packet_npc_info *src);

void packet_status_to(struct packet *dest, struct packet_status *src)
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

void packet_enter_world_to(struct packet *dest, struct packet_enter_world *src);

void packet_say_to(struct packet *dest, struct packet_say *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x4a);
    packet_write_u32(dest, src->obj_id);
    packet_write_u32(dest, src->type);
    packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
    packet_write(dest, src->message.buf, l2_string_size(src->message.buf));
}

void packet_attack_to(struct packet *dest, struct packet_attack *src)
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

void packet_auto_attack_stop_to(struct packet *dest, struct packet_auto_attack_stop *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x2c);
    packet_write_u32(dest, src->target_id);
}

void packet_unselect_target_to(struct packet *dest, struct packet_unselect_target *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x2a);
    packet_write_u32(dest, src->target_id);
    packet_write_i32(dest, src->target_pos.x);
    packet_write_i32(dest, src->target_pos.y);
    packet_write_i32(dest, src->target_pos.z);
}

void packet_die_to(struct packet *dest, struct packet_die *src)
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

void packet_npc_html_message_to(struct packet *dest, struct packet_npc_html_message *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x0f);
    packet_write_u32(dest, src->message_id);
    packet_write(dest, src->message.buf, l2_string_size(src->message.buf));
}

void packet_change_move_type_to(struct packet *dest, struct packet_change_move_type *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x2e);
    packet_write_u32(dest, src->obj_id);
    packet_write_u32(dest, src->running);
    packet_write_u32(dest, 0);
}

void packet_skill_list_to(struct packet *dest, struct packet_skill_list *src)
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

void packet_skill_use_to(struct packet *dest, struct packet_skill_use *src)
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
