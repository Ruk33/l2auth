#ifndef PACKET_ENCODER_H
#define PACKET_ENCODER_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "l2_string.h"
#include "types.h"

struct protocol_version {
	byte content[9];
};

struct d0 {
	u32 manor_size;
};

struct select_character {
	u32 id;
	u32 play_ok1;
	u32 race_id;
	u32 class_id;
	u32 clan_id;
	struct l2_character_name name;
	struct l2_character_title title;
	u32 sex;
	u32 active;
	struct v3 position;
	double hp;
	double mp;
	u32 sp;
	u32 exp;
	u32 level;
	struct attrs attrs;
	u32 game_time;
};

struct quest_list {
	byte empty[7];
};

struct validate_position {
	u32 id;
	struct v3 position;
	i32 heading;
};

struct move {
	u32 id;
	struct v3 prev_position;
	struct v3 new_position;
};

struct restart {
	u32 response;
};

struct revive {
	u32 obj_id;
};

struct select_target {
	u32 target_id;
	u32 color;
};

struct auth_login {
	u32 count;
	// TODO: confirm how many characters a user can have in the lobby.
	struct character characters[10];
};

struct character_info {
	u32 id;
	struct v3 position;
	i32 heading;
	struct l2_character_name name;
	struct l2_character_title title;
	u32 race_id;
	u32 sex;
	u32 class_id;
	u32 pvp_flag;
	u32 karma;
	u32 m_attack_speed;
	u32 p_attack_speed;
	u32 run_speed;
	u32 walk_speed;
	u32 swim_run_speed;
	u32 swim_walk_speed;
	u32 fly_run_speed;
	u32 fly_walk_speed;
	double movement_multiplier;
	double attack_speed_multiplier;
	double collision_radius;
	double collision_height;
	u32 hair_style_id;
	u32 hair_color_id;
	u32 face_id;
	u32 clan_id;
	u32 clan_crest_id;
	u32 ally_id;
	u32 ally_crest_id;
	u8 standing;
	u8 running;
	u8 in_combat;
	u8 alike_dead;
	u8 invisible;
	u8 mount_type;
	u8 private_store_type;
	u16 cubics;
	u8 find_party_member;
	u32 abnormal_effect;
	u16 recommendations_left;
	u8 mounted;
	u32 clan_crest_large_id;
	u8 hero_symbol;
	u8 hero_aura;
	u8 fishing;
	u32 name_color;
};

struct show_creation_screen {
	u32 count;
	struct character_template templates[10];
};

struct create_character {
	u32 response;
};

struct npc_info {
	u32 id;
	u32 template_id;
	u32 attackable;
	struct v3 position;
	i32 heading;
	u32 m_attack_speed;
	u32 p_attack_speed;
	u32 run_speed;
	u32 walk_speed;
	u32 swim_run_speed;
	u32 swim_walk_speed;
	u32 fly_run_speed;
	u32 fly_walk_speed;
	double magic_multiplier;
	double movement_multiplier;
	double collision_radius;
	double collision_height;
	u32 r_hand_id;
	u32 l_hand_id;
	u8 name_above_char;
	u8 running;
	u8 in_combat;
	u8 alike_dead;
	u8 summoned;
	struct l2_character_name name;
	struct l2_character_title title;
	u32 karma;
	u32 abnormal_effect;
	u8 team_circle;
};

enum status_type
{
	STATUS_LEVEL = 0x01,
	STATUS_EXP   = 0x02,
	STATUS_STR   = 0x03,
	STATUS_DEX   = 0x04,
	STATUS_CON   = 0x05,
	STATUS_INT   = 0x06,
	STATUS_WIT   = 0x07,
	STATUS_MEN   = 0x08,

	STATUS_CUR_HP = 0x09,
	STATUS_MAX_HP = 0x0a,
	STATUS_CUR_MP = 0x0b,
	STATUS_MAX_MP = 0x0c,

	STATUS_SP		= 0x0d,
	STATUS_CUR_LOAD	= 0x0e,
	STATUS_MAX_LOAD	= 0x0f,

	STATUS_P_ATK	= 0x11,
	STATUS_ATK_SPD	= 0x12,
	STATUS_P_DEF	= 0x13,
	STATUS_EVASION	= 0x14,
	STATUS_ACCURACY	= 0x15,
	STATUS_CRITICAL	= 0x16,
	STATUS_M_ATK	= 0x17,
	STATUS_CAST_SPD	= 0x18,
	STATUS_M_DEF	= 0x19,
	STATUS_PVP_FLAG	= 0x1a,
	STATUS_KARMA	= 0x1b,

	STATUS_CUR_CP = 0x21,
	STATUS_MAX_CP = 0x22,
};

struct status {
	enum status_type type;
	u32 value;
};

struct status_list {
	u32 obj_id;
	struct status statuses[35];
	u32 count;
};

struct enter_world {
	u32 id;
	struct v3 position;
	i32 heading;
	struct l2_character_name name;
	u32 race_id;
	u32 sex;
	u32 class_id;
	u32 level;
	u32 exp;
	struct attrs attrs;
	u32 max_hp;
	u32 hp;
	u32 max_mp;
	u32 mp;
	u32 sp;
	u32 current_load;
	u32 max_load;
	u32 paperdoll_under_id;
	u32 paperdoll_r_ear_id;
	u32 paperdoll_l_ear_id;
	u32 paperdoll_neck_id;
	u32 paperdoll_r_finger_id;
	u32 paperdoll_l_finger_id;
	u32 paperdoll_head_id;
	u32 paperdoll_r_hand_id;
	u32 paperdoll_l_hand_id;
	u32 paperdoll_gloves_id;
	u32 paperdoll_chest_id;
	u32 paperdoll_legs_id;
	u32 paperdoll_feet_id;
	u32 paperdoll_back_id;
	u32 paperdoll_lr_hand_id;
	u32 paperdoll_hair_id;
	u32 p_attack;
	u32 p_attack_speed;
	u32 p_def;
	u32 evasion_rate;
	u32 accuracy;
	u32 critical_hit;
	u32 m_attack;
	u32 m_attack_speed;
	u32 m_def;
	u32 pvp_flag;
	u32 karma;
	u32 run_speed;
	u32 walk_speed;
	u32 swim_run_speed;
	u32 swim_walk_speed;
	u32 fly_run_speed;
	u32 fly_walk_speed;
	u32 fly_run_speed_copy;
	u32 fly_walk_speed_copy;
	double movement_speed_multiplier;
	double attack_speed_multiplier;
	double collision_radius;
	double collision_height;
	u32 hair_style_id;
	u32 hair_color_id;
	u32 face_id;
	i32 access_level;
	struct l2_character_title title;
	u32 clan_id;
	u32 clan_crest_id;
	u32 ally_id;
	u32 ally_crest_id;
	u32 clan_leader;
	u8 mount_type;
	u8 private_store_type;
	u8 dwarven_craft;
	u32 pk_kills;
	u32 pvp_kills;
	u16 cubics;
	u8 party_members;
	u32 abnormal_effect;
	u32 clan_privileges;
	u16 recommendation_left;
	u16 recommendation_have;
	u16 inventory_limit;
	u32 max_cp;
	u32 cp;
	u8 mounted;
	u8 team_circle;
	u32 clan_crest_large_id;
	u8 hero_symbol;
	u8 hero;
	i32 fish_x;
	i32 fish_y;
	i32 fish_z;
	u32 name_color;
};

struct say {
	u32 obj_id;
	u32 type;
	struct l2_character_name name;
	struct l2_character_message message;
};

struct attack_hit {
	u32 target_id;
	u32 damage;
	u8 flags;
};

struct attack {
	u32 attacker_id;
	struct v3 attacker_pos;
	struct attack_hit hits[10];
	u16 hit_count;
};

struct stop_auto_attack {
	u32 target_id;
};

struct deselect_target {
	u32 target_id;
	struct v3 target_position;
};

struct die {
	u32 obj_id;
	u32 to_village;
	u32 to_hideaway;
	u32 to_castle;
	u32 to_siege;
	u32 sweepable;
	u32 to_fixed;
};

struct npc_html_message {
	u32 message_id;
	struct l2_npc_dialog message;
};

struct change_move_type {
	u32 obj_id;
	u32 running;
};

struct skill {
	u32 id;
	u32 passive;
	u32 level;
};

struct skill_list {
	struct skill skills[32];
	u32 count;
};

struct use_skill {
	u32 src_id;
	u32 target_id;
	u32 skill_id;
	u32 skill_level;
	u32 hit_time;
	u32 reuse_delay;
	struct v3 position;
};

void encode_protocol_version(struct packet *dest, struct protocol_version *src);

void encode_d0(struct packet *dest, struct d0 *src);

void encode_select_character(struct packet *dest, struct select_character *src);

void encode_quest_list(struct packet *dest, struct quest_list *src);

void encode_validate_position(struct packet *dest, struct validate_position *src);

void encode_move(struct packet *dest, struct move *src);

void encode_restart(struct packet *dest, struct restart *src);

void encode_revive(struct packet *dest, struct revive *src);

void encode_select_target(struct packet *dest, struct select_target *src);

void encode_auth_login(struct packet *dest, struct auth_login *src);

void encode_character_info(struct packet *dest, struct character_info *src);

void encode_show_creation_screen(struct packet *dest, struct show_creation_screen *src);

void encode_create_character(struct packet *dest, struct create_character *src);

void encode_npc_info(struct packet *dest, struct npc_info *src);

void encode_status_list(struct packet *dest, struct status_list *src);

void encode_enter_world(struct packet *dest, struct enter_world *src);

void encode_say(struct packet *dest, struct say *src);

void encode_attack(struct packet *dest, struct attack *src);

void encode_stop_auto_attack(struct packet *dest, struct stop_auto_attack *src);

void encode_deselect_target(struct packet *dest, struct deselect_target *src);

void encode_die(struct packet *dest, struct die *src);

void encode_npc_html_message(struct packet *dest, struct npc_html_message *src);

void encode_change_move_type(struct packet *dest, struct change_move_type *src);

void encode_skill_list(struct packet *dest, struct skill_list *src);

void encode_use_skill(struct packet *dest, struct use_skill *src);

#endif
