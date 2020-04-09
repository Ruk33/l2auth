#ifndef L2AUTH_LOGIN_PACKET_GG_AUTH_C
#define L2AUTH_LOGIN_PACKET_GG_AUTH_C

#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <login/packet/gg_auth.h>

l2_packet* login_packet_gg_auth
(
        struct L2Client* client,
        enum login_packet_gg_auth_response response
)
{
        assert(client);
        l2_packet_type type = 0x0b;

        log_info("Creating gg auth packet");

        return l2_client_create_packet(
                client,
                type,
                (unsigned char*) &response,
                sizeof(response)
        );
}

#endif
