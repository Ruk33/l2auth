#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <login/server.h>
#include <login/client.h>
#include <login/packet/init.h>
#include <login/handler/new_connection.h>

void login_handler_new_connection
(
        struct LoginServer* server,
        struct LoginClient* client
)
{
        assert(server);
        assert(client);

        l2_packet* server_packet = login_packet_init(client);

        log_info("Handling new connection, sending init packet");
        login_client_send_packet(client, server_packet);
}
