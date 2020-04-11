#ifndef L2AUTH_GAME_HANDLER_PROTOCOL_VERSION_C
#define L2AUTH_GAME_HANDLER_PROTOCOL_VERSION_C

#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_client.h>
#include <core/l2_packet.h>
#include <game/server.h>
#include <game/packet/crypt_init.h>
#include <game/handler/protocol_version.h>

void game_handler_protocol_version
(
        struct GameServer* server,
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
                game_packet_crypt_init(client)
        );
}

#endif
