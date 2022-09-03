#ifndef PACKET_DECODER_H
#define PACKET_DECODER_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "types.h"

struct auth_login {
	struct username username;
	struct password password;
};

void decode_auth_login(struct auth_login *dest, struct packet *src);

#endif
