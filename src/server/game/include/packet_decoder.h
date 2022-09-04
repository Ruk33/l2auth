#ifndef PACKET_DECODER_H
#define PACKET_DECODER_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "l2_string.h"
#include "types.h"

struct action {
	u32 target_id;
	struct v3 position;
	u8 flags; // 0 = click, 1 = shift click
};

struct attack_request {
	u32 target_id;
	struct v3 position;
	u8 flags; // 0 = click, 1 = shift click
};

struct auth_request {
	struct l2_username username;
	u32 play_ok1;
	u32 play_ok2;
	u32 login_ok1;
	u32 login_ok2;
};

struct selected_character {
	u32 index;
};

struct create_character_request {
	struct l2_character_name name;
	u32 race_id;
	u32 sex;
	u32 class_id;
	struct attrs attrs;
	u32 hair_style_id;
	u32 hair_color_id;
	u32 face_id;
};

struct move_request {
	struct v3 position;
};

enum revive_option
{
	REVIVE_IN_CLAN_HALL	= 1,
	REVIVE_IN_CASTLE	= 2,
	REVIVE_IN_SIEGE_HQ	= 3,
	REVIVE_FIXED		= 4,
};

struct revive_request {
	enum revive_option option_chosen;
};

struct validate_position_request {
	struct v3 position;
	i32 heading;
};

struct say_request {
	// Check maximum length.
	l2_string message[128];
	size_t size;
};

struct bypass {
	// TODO: Check maximum size.
	l2_string command[128];
	size_t size;
};

struct use_skill_request {
	u32 skill_id;
	u32 ctrl_pressed;
	u8 shift_pressed;
};

void decode_action(struct action *dest, struct packet *src);

void decode_attack_request(struct attack_request *dest, struct packet *src);

void decode_auth_request(struct auth_request *dest, struct packet *src);

void decode_selected_character(struct selected_character *dest, struct packet *src);

void decode_create_character_request(struct create_character_request *dest, struct packet *src);

void decode_move_request(struct move_request *dest, struct packet *src);

void decode_revive_request(struct revive_request *dest, struct packet *src);

void decode_validate_position_request(struct validate_position_request *dest, struct packet *src);

void decode_say_request(struct say_request *dest, struct packet *src);

void decode_bypass(struct bypass *dest, struct packet *src);

void decode_use_skill_request(struct use_skill_request *dest, struct packet *src);

#endif
