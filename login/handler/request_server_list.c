#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_SERVER_LIST_C
#define L2AUTH_LOGIN_HANDLER_REQUEST_SERVER_LIST_C

#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <login/packet/server_list.h>
#include <login/handler/request_server_list.h>

void login_handler_request_server_list
(
        struct L2Server* server,
        struct L2Client* client
)
{
        log_info("Returning server list");
        l2_client_encrypt_and_send_packet(
                client,
                login_packet_server_list(client)
        );
}

#endif
