#ifndef TYPES_H
#define TYPES_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "l2_string.h"

typedef u32 in_game_id;

struct l2_username {
	l2_string buf[28];
};

struct l2_character_name {
	// Check if this limit is ok.
	l2_string buf[32];
};

struct l2_character_title {
	// Check if this limit is ok.
	l2_string buf[28];
};

struct l2_character_message {
	// Double check max message len sent from a char.
	l2_string buf[256];
};

struct l2_npc_dialog {
	// 4k limit.
	l2_string buf[2048];
};

struct character_position {
	i32 x;
	i32 y;
	i32 z;
	i32 heading;
};

struct character_attr {
	i32 _int;
	i32 str;
	i32 con;
	i32 men;
	i32 dex;
	i32 wit;
};

enum character_state {
	IDLE,
	MOVING,
};

struct character {
	in_game_id id;
	enum character_state state;
	struct character_position position;
	struct character_position moving_to;
	u32 run_speed;
};

struct crypt_key {
	byte buf[8];
};

struct client {
	size_t id;
	struct character *character;
	struct crypt_key encrypt_key;
	struct crypt_key decrypt_key;
	struct packet response;
	struct packet request;
	size_t received;
	int partial;
	struct packet response_queue[8];
	size_t response_queue_count;
	int conn_encrypted;
};

struct state {
	struct client clients[1024];
	size_t recycled_clients[1024];
	size_t client_count;
	struct character characters[1024];
	size_t recycled_characters[1024];
	size_t character_count;
};

#endif
