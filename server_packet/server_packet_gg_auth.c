#ifndef L2AUTH_SERVER_PACKET_GG_AUTH_C
#define L2AUTH_SERVER_PACKET_GG_AUTH_C

#include <core/l2_packet.c>

enum gg_auth_response
{
        GG_AUTH_RESPONSE_SKIP_GG = 0x0b
};

l2_packet* server_packet_gg_auth(enum gg_auth_response response)
{
        l2_packet_type type = 0x0b;

        return l2_packet_new(
                type,
                (unsigned char*) &response,
                sizeof(response)
        );
}

#endif //L2AUTH_SERVER_PACKET_GG_AUTH_C
