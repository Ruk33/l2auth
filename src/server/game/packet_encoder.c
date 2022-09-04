#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/l2_string.h"
#include "include/packet_encoder.h"

void encode_protocol_version(struct packet *dest, struct protocol_version *src)
{
	assert(dest);
	assert(src);
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
	packet_set_type(dest, 0x00);
	packet_write(dest, content, sizeof(content));
	packet_write_u16(dest, 0);
}

void encode_d0(struct packet *dest, struct d0 *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x1B);
	packet_write_u16(dest, 0);
	packet_write_u32(dest, src->manor_size);
}

void encode_select_character(struct packet *dest, struct select_character *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x15);
	packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
	packet_write_u32(dest, src->id);
	packet_write(dest, src->title.buf, l2_string_size(src->title.buf));
	packet_write_u32(dest, src->play_ok1);
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
	for (int i = 0; i < 37; i += 1)
		packet_write_u32(dest, 0);
	packet_write_u32(dest, src->game_time);
	for (int i = 0; i < 16; i += 1)
		packet_write_u32(dest, 0);
}

void encode_quest_list(struct packet *dest, struct quest_list *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x80);
	packet_write(dest, src->empty, sizeof(src->empty));
}

void encode_validate_position(struct packet *dest, struct validate_position *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x61);
	packet_write_u32(dest, src->id);
	packet_write_i32(dest, src->position.x);
	packet_write_i32(dest, src->position.y);
	packet_write_i32(dest, src->position.z);
	packet_write_i32(dest, src->heading);
}

void encode_move(struct packet *dest, struct move *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x01);
	packet_write_u32(dest, src->id);
	packet_write_i32(dest, src->new_position.x);
	packet_write_i32(dest, src->new_position.y);
	packet_write_i32(dest, src->new_position.z);
	packet_write_i32(dest, src->prev_position.x);
	packet_write_i32(dest, src->prev_position.y);
	packet_write_i32(dest, src->prev_position.z);
}

void encode_restart(struct packet *dest, struct restart *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x5f);
	packet_write_u32(dest, src->response);
}

void encode_revive(struct packet *dest, struct revive *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x07);
	packet_write_u32(dest, src->obj_id);
}

void encode_select_target(struct packet *dest, struct select_target *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0xa6);
	packet_write_u32(dest, src->target_id);
	packet_write_u32(dest, src->color);
}

void encode_auth_login(struct packet *dest, struct auth_login *src)
{
	assert(dest);
	assert(src);

	packet_set_type(dest, 0x13);
	packet_write_u32(dest, src->count);

	for (u32 i = 0; i < src->count; i += 1) {
		struct character *character = &src->characters[i];

		packet_write(dest, character->name.buf, l2_string_size(character->name.buf));
		packet_write_u32(dest, character->id);
		packet_write(dest, character->name.buf, l2_string_size(character->name.buf));
		packet_write_u32(dest, character->play_ok1);
		packet_write_u32(dest, character->clan_id);
		packet_write_u32(dest, 0);
		packet_write_u32(dest, character->sex);
		packet_write_u32(dest, character->race_id);
		packet_write_u32(dest, character->class_id);
		packet_write_u32(dest, character->active);
		packet_write_i32(dest, character->position.x);
		packet_write_i32(dest, character->position.y);
		packet_write_i32(dest, character->position.z);
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
		packet_write_d(dest, character->max_hp);
		packet_write_d(dest, character->max_mp);
		packet_write_u32(dest, character->delete_days);
		packet_write_u32(dest, character->class_id);
		packet_write_u32(dest, character->auto_select);
		packet_write_u8(dest, character->enchant_effect);
	}
}

void encode_character_info(struct packet *dest, struct character_info *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x03);
	packet_write_i32(dest, src->position.x);
	packet_write_i32(dest, src->position.y);
	packet_write_i32(dest, src->position.z);
	packet_write_i32(dest, src->heading);
	packet_write_u32(dest, src->id);
	packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
	packet_write_u32(dest, src->race_id);
	packet_write_u32(dest, src->sex);
	packet_write_u32(dest, src->class_id);
	packet_write_u32(dest, 0);
	// Paperdoll?
	for (int i = 0; i < 10; i += 1)
		packet_write_u32(dest, 0);
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
	packet_write_u32(dest, src->hair_style_id);
	packet_write_u32(dest, src->hair_color_id);
	packet_write_u32(dest, src->face_id);
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

void encode_show_creation_screen(struct packet *dest, struct show_creation_screen *src)
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

void encode_create_character(struct packet *dest, struct create_character *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x19);
	packet_write_u32(dest, src->response);
}

void encode_npc_info(struct packet *dest, struct npc_info *src)
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
	packet_write_i32(dest, src->heading);
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
	packet_write_u32(dest, src->r_hand_id);
	packet_write_u32(dest, 0);
	packet_write_u32(dest, src->l_hand_id);
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

void encode_status_list(struct packet *dest, struct status_list *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x0e);
	packet_write_u32(dest, src->obj_id);
	packet_write_u32(dest, src->count);
	for (u32 i = 0; i < src->count; i += 1) {
		packet_write_u32(dest, src->statuses[i].type);
		packet_write_u32(dest, src->statuses[i].value);
	}
}

void encode_enter_world(struct packet *dest, struct enter_world *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x04);
	packet_write_i32(dest, src->position.x);
	packet_write_i32(dest, src->position.y);
	packet_write_i32(dest, src->position.z);
	packet_write_i32(dest, src->heading);
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
	packet_write_d(dest, src->movement_speed_multiplier);
	packet_write_d(dest, src->attack_speed_multiplier);
	packet_write_d(dest, src->collision_radius);
	packet_write_d(dest, src->collision_height);
	packet_write_u32(dest, src->hair_style_id);
	packet_write_u32(dest, src->hair_color_id);
	packet_write_u32(dest, src->face_id);
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
	for (int i = 0; i < 7; i += 1)
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

void encode_say(struct packet *dest, struct say *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x4a);
	packet_write_u32(dest, src->obj_id);
	packet_write_u32(dest, src->type);
	packet_write(dest, src->name.buf, l2_string_size(src->name.buf));
	packet_write(dest, src->message.buf, l2_string_size(src->message.buf));
}

void encode_attack(struct packet *dest, struct attack *src)
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

void encode_stop_auto_attack(struct packet *dest, struct stop_auto_attack *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x2c);
	packet_write_u32(dest, src->target_id);
}

void encode_deselect_target(struct packet *dest, struct deselect_target *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x2a);
	packet_write_u32(dest, src->target_id);
	packet_write_i32(dest, src->target_position.x);
	packet_write_i32(dest, src->target_position.y);
	packet_write_i32(dest, src->target_position.z);
}

void encode_die(struct packet *dest, struct die *src)
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

void encode_npc_html_message(struct packet *dest, struct npc_html_message *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x0f);
	packet_write_u32(dest, src->message_id);
	packet_write(dest, src->message.buf, l2_string_size(src->message.buf));
}

void encode_change_move_type(struct packet *dest, struct change_move_type *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x2e);
	packet_write_u32(dest, src->obj_id);
	packet_write_u32(dest, src->running);
	packet_write_u32(dest, 0);
}

void encode_skill_list(struct packet *dest, struct skill_list *src)
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

void encode_use_skill(struct packet *dest, struct use_skill *src)
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
