#ifndef SESSION_H
#define SESSION_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "types.h"

void init_session(struct session *src);

void decrypt_packet(struct session *session, struct packet *src);

void encrypt_packet(struct session *session, struct packet *src);

#endif
