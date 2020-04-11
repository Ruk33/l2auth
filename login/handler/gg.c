#ifndef L2AUTH_LOGIN_HANDLER_GG_C
#define L2AUTH_LOGIN_HANDLER_GG_C

#include <assert.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <login/packet/gg_auth.h>
#include <login/handler/gg.h>

void login_handler_gg
(
        struct L2Server* server,
        struct L2Client* client
)
{
        assert(server);
        assert(client);

        l2_packet* response = login_packet_gg_auth(
                client,
                LOGIN_PACKET_GG_AUTH_RESPONSE_SKIP_GG
        );
        l2_client_encrypt_and_send_packet(client, response);
}

#endif
