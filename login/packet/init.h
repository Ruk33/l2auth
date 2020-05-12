#ifndef L2AUTH_LOGIN_PACKET_INIT_H
#define L2AUTH_LOGIN_PACKET_INIT_H

#include <core/l2_packet.h>
#include <login/client.h>

l2_packet* login_packet_init(struct LoginClient* client);

#endif
