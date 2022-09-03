#ifndef PACKET_ENCODER_H
#define PACKET_ENCODER_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "session.h"

struct session_id {
	byte buf[4];
};

struct protocol {
	byte buf[4];
};

struct init {
	struct session_id session_id;
	struct protocol protocol;
	struct rsa_modulus modulus;
};

struct gg_auth {
	u32 gg_response;
};

struct ok {
	u32 login_ok1;
	u32 login_ok2;
};

struct play_ok {
	u32 play_ok1;
	u32 play_ok2;
};

struct server {
	u8 id;
	u32 ip;
	u32 port;
	u8 age_limit;
	u8 pvp;
	u16 players;
	u16 max_players;
	u8 status;
	u32 extra;
	u8 brackets;
};

struct server_list {
	u8 count;
	struct server servers[8];
};

void encode_init(struct packet *dest, struct init *src);

void encode_gg_auth(struct packet *dest, struct gg_auth *src);

void encode_ok(struct packet *dest, struct ok *src);

void encode_play_ok(struct packet *dest, struct play_ok *src);

void encode_server_list(struct packet *dest, struct server_list *src);

#endif
