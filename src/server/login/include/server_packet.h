#ifndef SERVER_PACKET_H
#define SERVER_PACKET_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "client.h"

struct session_id {
	byte buf[4];
};

struct protocol {
	byte buf[4];
};

struct packet_init {
	struct session_id session_id;
	struct protocol protocol;
	struct client_modulus modulus;
};

struct packet_gg_auth {
	i32 gg_response;
};

struct packet_ok {
	i32 loginOK1;
	i32 loginOK2;
};

struct packet_play_ok {
	i32 playOK1;
	i32 playOK2;
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

struct packet_server_list {
	u8 count;
	struct server servers[8];
};

void packet_init_to(struct packet *dest, struct packet_init *src);

void packet_gg_auth_to(struct packet *dest, struct packet_gg_auth *src);

void packet_ok_to(struct packet *dest, struct packet_ok *src);

void packet_play_ok_to(struct packet *dest, struct packet_play_ok *src);

void packet_server_list_to(struct packet *dest, struct packet_server_list *src);

#endif
