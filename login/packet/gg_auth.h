#ifndef L2AUTH_LOGIN_PACKET_GG_AUTH_H
#define L2AUTH_LOGIN_PACKET_GG_AUTH_H

#include <core/l2_packet.h>

enum login_packet_gg_auth_response
{
        LOGIN_PACKET_GG_AUTH_RESPONSE_SKIP_GG = 0x0b
};

l2_packet* login_packet_gg_auth
(
        enum login_packet_gg_auth_response response
);

#endif
