#ifndef TYPES_H
#define TYPES_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "l2_string.h"

typedef u32 in_game_id;

struct username {
	i8 buf[14];
};

struct l2_username {
	l2_string buf[16];
};

struct l2_character_name {
	// Check if this limit is ok.
	l2_string buf[16];
};

struct l2_character_title {
	// Check if this limit is ok.
	l2_string buf[16];
};

struct l2_character_message {
	// Double check max message len sent from a char.
	l2_string buf[128];
};

struct l2_npc_dialog {
	// 4k limit.
	l2_string buf[2048];
};

struct v3 {
	i32 x;
	i32 y;
	i32 z;
};

struct attrs {
	i32 _int;
	i32 str;
	i32 con;
	i32 men;
	i32 dex;
	i32 wit;
};

struct character_template {
	u32 race_id;
	u32 class_id;
	struct attrs attrs;
};

enum character_state {
	IDLE,
	MOVING,
};

struct character {
	in_game_id id;
	enum character_state state;
	struct v3 position;
	struct v3 velocity;
	struct v3 moving_to;
	struct l2_character_name name;
	struct l2_character_title title;
	u32 play_ok1;
	u32 clan_id;
	u32 sex;
	u32 race_id;
	u32 class_id;
	u32 active;
	double hp;
	double mp;
	double max_hp;
	double max_mp;
	u32 cp;
	u32 max_cp;
	struct attrs attrs;
	u32 sp;
	u32 exp;
	u32 level;
	u32 karma;
	u32 under_obj_id;
	u32 r_ear_obj_id;
	u32 l_ear_obj_id;
	u32 neck_obj_id;
	u32 r_finger_obj_id;
	u32 l_finger_obj_id;
	u32 head_obj_id;
	u32 r_hand_obj_id;
	u32 l_hand_obj_id;
	u32 gloves_obj_id;
	u32 chest_obj_id;
	u32 legs_obj_id;
	u32 feet_obj_id;
	u32 back_obj_id;
	u32 lr_hand_obj_id;
	u32 hair_obj_id;
	u32 under_id;
	u32 r_ear_id;
	u32 l_ear_id;
	u32 neck_id;
	u32 r_finger_id;
	u32 l_finger_id;
	u32 head_id;
	u32 r_hand_id;
	u32 l_hand_id;
	u32 gloves_id;
	u32 chest_id;
	u32 legs_id;
	u32 feet_id;
	u32 back_id;
	u32 lr_hand_id;
	u32 hair_id;
	u32 hair_style_id;
	u32 hair_color_id;
	u32 face_id;
	u32 delete_days;
	u32 auto_select;
	u8 enchant_effect;
	u32 p_attack;
	u32 m_attack;
	u32 p_def;
	u32 m_def;
	u32 evasion;
	u32 accuracy;
	u32 critical_hit;
	u16 inventory_limit;
	u32 run_speed;
	u32 walk_speed;
	u32 p_attack_speed;
	u32 m_attack_speed;
	double movement_speed_multiplier;
	double attack_speed_multiplier;
	double collision_radius;
	double collision_height;
	u32 name_color;
	u32 current_load;
	u32 max_load;
};

struct crypt_key {
	byte buf[8];
};

struct session {
	u32 id;
	struct username username;
	struct character *character;
	struct crypt_key encrypt_key;
	struct crypt_key decrypt_key;
	struct packet response;
	struct packet request;
	size_t received;
	int partial;
	// struct packet response_queue[8];
	// size_t response_queue_count;
	int conn_encrypted;
};

struct state {
	struct session sessions[1024];
	struct character characters[1024];
};

#endif
