#ifndef CLIENT_PACKET_H
#define CLIENT_PACKET_H

#include "../../../include/util.h"
#include "../../../include/packet.h"

struct l2_username {
	i8 buf[14];
};

struct password {
	i8 buf[16];
};

struct server_packet_auth_login {
	struct l2_username username;
	struct password password;
};

void packet_auth_login_from(struct server_packet_auth_login *dest, struct packet *src);

#endif
