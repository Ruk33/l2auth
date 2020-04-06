#ifndef L2AUTH_LOGIN_PACKET_OK_H
#define L2AUTH_LOGIN_PACKET_OK_H

#include <core/l2_packet.h>
#include <login/dto/session_key.h>

l2_packet* login_packet_ok(struct LoginDtoSessionKey* session_key);

#endif
