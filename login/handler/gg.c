#include <assert.h>
#include <login/server.h>
#include <login/client.h>
#include <login/packet/gg_auth.h>
#include <login/handler/gg.h>

void login_handler_gg
(
        struct LoginServer* server,
        struct LoginClient* client
)
{
        assert(server);
        assert(client);

        l2_packet* response = login_packet_gg_auth(
                client,
                LOGIN_PACKET_GG_AUTH_RESPONSE_SKIP_GG
        );
        login_client_encrypt_and_send_packet(client, response);
}
