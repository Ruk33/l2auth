#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_packet.h>
#include "response.h"
#include "handler.h"

void game_action_protocol_version_handler
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request
)
{
        assert(server);
        assert(client);
        assert(request);

        int protocol = 0;

        log_info("Handling protocol version packet");

        memcpy(
                &protocol,
                request + 3 /* 2 = packet size, 1 = packet type */,
                sizeof(protocol)
        );
        
        log_info("Protocol version %d", protocol);
        log_info("Returning crypt init");

        l2_client_send_packet(
                client,
                game_action_protocol_version_response(client)
        );
}
