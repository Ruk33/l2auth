#ifndef L2AUTH_LOGIN_PACKET_GG_AUTH_C
#define L2AUTH_LOGIN_PACKET_GG_AUTH_C

#include <core/l2_packet.h>
#include <login/packet/gg_auth.h>

l2_packet* login_packet_gg_auth
(
        enum login_packet_gg_auth_response response
)
{
        l2_packet_type type = 0x0b;

        return l2_packet_new(
                type,
                (unsigned char*) &response,
                sizeof(response)
        );
}

#endif
