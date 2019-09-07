#ifndef L2AUTH_LOGIN_PACKET_GG_AUTH_C
#define L2AUTH_LOGIN_PACKET_GG_AUTH_C

#include <core/l2_packet.c>

enum login_packet_gg_auth_response
{
        LOGIN_PACKET_GG_AUTH_RESPONSE_SKIP_GG = 0x0b
};

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
