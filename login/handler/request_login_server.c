#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <login/server.h>
#include <login/client.h>
#include <login/packet/play_ok.h>
#include <login/handler/request_login_server.h>

void login_handler_request_login_server
(
        struct LoginServer* server,
        struct LoginClient* client
)
{
        assert(server);
        assert(client);
        log_info("Client wants to log into server");
        l2_packet* packet = login_packet_play_ok(client);
        login_client_encrypt_and_send_packet(client, packet);
}
