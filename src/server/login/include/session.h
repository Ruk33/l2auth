#ifndef SESSION_H
#define SESSION_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "types.h"

struct rsa_modulus {
	byte buf[128];
};

struct request {
	struct packet packet;
	u32 received;
	int is_partial;
};

struct session {
	int active;
	u32 id;
	struct username username;
	struct request request;
	struct packet response;
	u32 play_ok1;
	u32 play_ok2;
};

// On success, 1 is returned.
// On error, 0 is returned.
int session_init(struct session *src);

// Release resources used by client
// so it can be re-used by another connection.
void session_release(struct session *src);

// On success, 1 is returned.
// On error, 0 is returned.
int session_rsa_modulus(struct rsa_modulus *dest, struct session *src);

// On success, 1 is returned.
// On error, 0 is returned.
int session_encrypt_packet(struct session *session, struct packet *src);

// On success, 1 is returned.
// On error, 0 is returned.
int session_decrypt_packet(struct session *session, struct packet *src);

#endif
