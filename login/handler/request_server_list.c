#include <log/log.h>
#include <core/l2_packet.h>
#include <login/server.h>
#include <login/client.h>
#include <login/packet/server_list.h>
#include <login/handler/request_server_list.h>

void login_handler_request_server_list
(
        struct LoginServer* server,
        struct LoginClient* client
)
{
        log_info("Returning server list");
        login_client_encrypt_and_send_packet(
                client,
                login_packet_server_list(client)
        );
}
