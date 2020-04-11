#include <assert.h>
#include <log/log.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_packet.h>
#include <login/packet/init.h>
#include <login/handler/new_connection.h>

void login_handler_new_connection
(
        struct L2Server* server,
        struct L2Client* client
)
{
        assert(server);
        assert(client);

        l2_packet* server_packet = login_packet_init(client);

        log_info("Handling new connection, sending init packet");
        l2_client_send_packet(client, server_packet);
}
