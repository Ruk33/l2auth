#ifndef TYPES_H
#define TYPES_H

#include "../../../include/util.h"
#include "l2_string.h"

typedef u32 in_game_id;

struct l2_username {
	l2_string buf[28];
};

struct l2_char_name {
	// Check if this limit is ok.
	l2_string buf[32];
};

struct l2_char_title {
	// Check if this limit is ok.
	l2_string buf[28];
};

struct l2_char_message {
	// Double check max message len sent from a char.
	l2_string buf[256];
};

struct l2_npc_html_message {
	// 4k limit.
	l2_string buf[2048];
};

struct char_pos {
	i32 x;
	i32 y;
	i32 z;
	i32 heading;
};

struct char_attr {
	i32 _int;
	i32 str;
	i32 con;
	i32 men;
	i32 dex;
	i32 wit;
};

struct character {
	in_game_id id;
};

#endif
