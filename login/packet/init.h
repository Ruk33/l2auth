#ifndef L2AUTH_LOGIN_PACKET_INIT_H
#define L2AUTH_LOGIN_PACKET_INIT_H

#include <core/l2_rsa_key.h>
#include <core/l2_packet.h>

l2_packet* login_packet_init(struct L2RSAKey *rsa_key);

#endif
